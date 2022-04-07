#include "discorduserapi.h"

#include "discordgateway.h"

#include "logger.h"

string url_encode(const string& value) {
	ostringstream escaped;
	escaped.fill('0');
	escaped << hex;

	for (const char c : value)
    {
        // Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << uppercase;
		escaped << '%' << setw(2) << int((unsigned char)c);
		escaped << nouppercase;
	}

	return escaped.str();
}

#define DISCORD_STATUS_DND "dnd"
#define DISCORD_STATUS_ONLINE "online"
#define DISCORD_STATUS_IDLE "idle"
#define DISCORD_STATUS_INVISIBLE "invisible"
#define DISCORD_STATUS_OFFLINE "offline"

discordgateway* gateway;

discordclient::discordclient(const string token, const bool is_bot): token_(token), is_bot_(is_bot) {
	headers_ = {
		{"Authorization", is_bot ? string("Bot ").append(token).c_str() : token.c_str()},
		{"user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36 Edg/97.0.1072.76"},
		{"content-type", "application/json"}
	};

	client_ = new httplib::Client(string("https://discord.com"));
	client_->set_default_headers(headers_);
}

void discordclient::setup_gateway() {
	gateway = new discordgateway(token_, this);
	gateway->connect();
}

bool discordclient::send_message(string channel_id, string content, bool tts) {

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<long long unsigned> distribution(10000000000000000, 99999999999999999);

	json j;

	j["content"] = content;
	j["nonce"] = to_string(distribution(generator)).c_str();
	j["tts"] = false;

	auto response = client_->Post((string("/api/v9/channels/") + channel_id + string("/messages")).c_str(), j.dump().c_str(), "application/json");

	return response->status == 200;
}

bool discordclient::send_message(const long long channel_id, const string content, const bool tts) {

	return send_message(to_string(channel_id), content, tts);
}

bool discordclient::set_status(const string text) {
	return set_status(text, "");
}

bool discordclient::set_status(string text, string emote) {
	json j;

	j["custom_status"]["text"] = text;
	if(!emote.empty())
		j["custom_status"]["emoji_name"] = "🧀";

	auto response = client_->Patch("/api/v9/users/@me/settings", j.dump().c_str(), "application/json");

	return response->status == 200;
}

bool discordclient::set_online_status(string status) {
	json j;

	j["status"] = status;

	auto response = client_->Patch("/api/v9/users/@me/settings", j.dump().c_str(), "application/json");

	return response->status == 200;
}

bool discordclient::get_self_user(s_user* p_user) {
	auto response = client_->Get("/api/v9/users/@me");

	if (response->status == 200) {
		*p_user = s_user::fromJson(json::parse(response->body));
		return true;
	}
	return false;
}

bool discordclient::get_user_by_id(string user_id, s_user* p_user) {

	s_user* cached_user = gateway->cache.get_user_from_id(user_id);
	if (cached_user) {
		*p_user = *cached_user;
		return true;
	}

	auto response = client_->Get(("/api/v9/users/" + user_id).c_str());

	if (response->status == 200) {
		*p_user = s_user::fromJson(json::parse(response->body));
		return true;
	}
	return false;
}

bool discordclient::get_guild_by_id(string guild_id, s_guild* p_guild) {

	s_guild* cached_guild = gateway->cache.get_guild_from_id(guild_id);
	if (cached_guild && !cached_guild->owner_id.empty()) {
		*p_guild = *cached_guild;
		return true;
	}

	auto response = client_->Get(("/api/v9/guilds/" + guild_id).c_str());

	if (response->status == 200) {
		json j = json::parse(response->body);
		logs::debug(response->body);
		*p_guild = s_guild::fromJson(j);
		gateway->cache.guilds.push_back(*p_guild);
		return true;
	}
	return false;
}

bool discordclient::get_guild_channels(string guild_id, vector<s_channel>* p_channel) {
	auto response = client_->Get(("/api/v9/guilds/" + guild_id + "/channels").c_str());

	if (response->status == 200) {
		json j = json::parse(response->body);
		for (json elem : j) {
			s_channel cur = s_channel::fromJson(elem);
			p_channel->push_back(cur);
			gateway->cache.channels.push_back(cur);
		}
		return true;
	}
	return false;
}

bool discordclient::get_channel_by_id(string channel_id, s_channel* p_channel) {

	s_channel* cached_channel = gateway->cache.get_channel_from_id(channel_id);
	if (cached_channel) {
		*p_channel = *cached_channel;
		return true;
	}

	auto response = client_->Get(("/api/v9/channels/" + channel_id).c_str());

	if (response->status == 200) {
		json j = json::parse(response->body);
		
		*p_channel = s_channel::fromJson(j);

		gateway->cache.channels.push_back(*p_channel);

		return true;
	}
	return false;
}

void discordclient::add_gateway_event(const char* eventname, const eventhandler hnd) {
	gateway->add_event(eventname, hnd);
}

void discordclient::print_event_handlers() {
	gateway->print_event_handlers();
}

void discordclient::search_messages(string guild_id, const string content, vector<s_message>* messages, int* total_results,
                                    const string author_id, const int offset)
{
	if (messages == nullptr || total_results == nullptr) {
		SetLastError(87);
		return;
	}
	string req = "/api/v9/guilds/" + url_encode(guild_id) + "/messages/search?content=" + url_encode(content);
	if (!author_id.empty())
		req += "&author_id=" + url_encode(author_id);
	if (offset != 0)
		req += "&offset=" + url_encode(to_string(offset));

	auto result = client_->Get(req.c_str());
	try {
		json j = json::parse(result->body);
			*total_results = j["total_results"];
			for (json msg : j["messages"]) {
				messages->push_back(s_message::fromJson(msg[0]));
			}
		}
	catch (json::exception e) {
		logs::error("Wrong body: " + result->body);
	}
}
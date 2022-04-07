#include "discordgateway.h"

#include <utility>

#include "logger.h"
#include "event_structs.h"

typedef eventhandler(*ready_event)();
typedef eventhandler(*message_create_event)(s_message);
typedef eventhandler(*presence_update_event)(gateway_events::presence);
typedef eventhandler(*voice_state_update_event)(s_voice_state);

void send_heartbeat(socketclient* c, websocketpp::connection_hdl hdl) {
	while (true) {
		c->send(hdl, "{\"op\": 1,\"d\" : 45000}", websocketpp::frame::opcode::text);
		Sleep(10000);
	}
}

discordgateway::discordgateway(string token, discordclient* dc_client) {
	this->token = std::move(token);
	this->dc_client = dc_client;
}

void discordgateway::connect() {
	client.set_tls_init_handler([this](websocketpp::connection_hdl) {
		return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv1);
		});
	try {
		//client.set_access_channels(websocketpp::log::alevel::all);
		client.set_access_channels(websocketpp::log::alevel::none);
		client.clear_access_channels(websocketpp::log::alevel::frame_payload);
		client.init_asio();

		client.set_message_handler([this, capture0 = &client](auto&& ph1, auto&& ph2)
        {
            on_message(capture0, std::forward<decltype(ph1)>(ph1), std::forward<decltype(ph2)>(ph2));
        });
		client.set_open_handler([this, capture0 = &client](auto&& ph1)
        {
            on_open(capture0, std::forward<decltype(ph1)>(ph1));
        });
		error_code err;
        const websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr con = client.get_connection(base_url, err);
		if (err) {
			logs::error("Couldn't connect to gateway: " + err.message());
			return;
		}
		client.connect(con);
		thread t(&socketclient::run, &client);
		t.detach();
		//websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
		//m_thread.reset(new websocketpp::lib::thread(&client::run, &client));
	}
	catch (websocketpp::exception const& e) {
		std::cout << e.what() << std::endl;
	}
}

map<string, vector<eventhandler>> event_handlers;

void discordgateway::on_message(socketclient* c, websocketpp::connection_hdl hdl, socketclient::message_ptr msg) {
	std::string payload = msg->get_payload();
	json j = json::parse(payload);
	if (j["op"] == 0) {
		string eventname = j["t"];
		if (eventname == string("READY")) {
			auto ready = gateway_events::ready::fromJson(j.at("d"));
			for (auto g : ready.guilds)
				cache.guilds.push_back(s_guild::fromUnavailableGuild(g));
			cache.users = ready.users;
		}
		else if (eventname == string("MESSAGE_CREATE")) {
			for (eventhandler hnd : event_handlers[eventname.c_str()]) {
				s_message message = s_message::fromJson(j["d"]);
				(message_create_event)(hnd)(message);
			}
		}
		else if (eventname == string("PRESENCE_UPDATE")) {
			for (eventhandler hnd : event_handlers[eventname.c_str()]) {
				gateway_events::presence event = gateway_events::presence::fromJson(j["d"]);
				(presence_update_event)(hnd)(event);
			}
		}
		else if (eventname == string("VOICE_STATE_UPDATE")) {
			for (eventhandler hnd : event_handlers[eventname.c_str()]) {
				s_voice_state state = s_voice_state::fromJson(j["d"]);
				(voice_state_update_event)(hnd)(state);
			}
		}
		else if (eventname == string("CHANNEL_CREATE")) {
		}
		else if (eventname == string("CHANNEL_UPDATE")) {
		}
		else if (eventname == string("CHANNEL_DELETE")) {
		}
		else if (eventname == string("MESSAGE_REACTION_ADD")) {
		}
		else if (eventname == string("MESSAGE_REACTION_REMOVE")) {
		}
		else {
			logs::debug("Unhandled Event: " + eventname + "\n");
		}
	}
}

void discordgateway::on_open(socketclient* c, websocketpp::connection_hdl hdl) {
	c->send(hdl, "{\"op\":2,\"d\":{\"token\":\"" + token + "\",\"capabilities\":32767,\"properties\":{\"os\":\"Windows\",\"browser\":\"Chrome\",\"device\":\"\",\"system_locale\":\"de-DE\",\"browser_user_agent\":\"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36\",\"browser_version\":\"97.0.4692.71\",\"os_version\":\"10\",\"referrer\":\"\",\"referring_domain\":\"\",\"referrer_current\":\"\",\"referring_domain_current\":\"\",\"release_channel\":\"stable\",\"client_build_number\":112644,\"client_event_source\":null},\"presence\":{\"status\":\"online\",\"since\":0,\"activities\":[],\"afk\":false},\"compress\":false,\"client_state\":{\"guild_hashes\":{},\"highest_last_message_id\":\"0\",\"read_state_version\":0,\"user_guild_settings_version\":-1,\"user_settings_version\":-1}}}", websocketpp::frame::opcode::text);
	//c->send(hdl, "{\"op\": 2, \"d\": {\"token\":\"" + token + "\", \"intents\":32767, \"properties\":{\"$os\": \"windows\", \"$browser\":\"tdiscord\", \"$device\":\"tdiscord\"}}}", websocketpp::frame::opcode::text);
	std::thread(send_heartbeat, c, hdl).detach();
}

void discordgateway::add_event(const char* eventname, eventhandler hnd) {
	event_handlers[string(eventname)].push_back(hnd);
}

int discordgateway::print_event_handlers() {
	int total = 0;
	for (pair < string, vector<eventhandler>> p : event_handlers) {
		logs::info("Registered " + to_string(p.second.size()) + " handlers for " + p.first);
		total += p.second.size();
	}
	return total;
}
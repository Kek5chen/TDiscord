#pragma once

#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include "structs.h"

using namespace std;

typedef void* eventhandler;

class discordclient {
private:
	string token_;
	bool is_bot_;
	httplib::Headers headers_;
	httplib::Client* client_;
public:
	discordclient(string token, bool is_bot);

	void setup_gateway();
	bool send_message(long long channel_id, string content, bool tts = false);
	bool send_message(string channel_id, string content, bool tts = false);
	bool set_status(string text);
	bool set_status(string text, string emote);
	bool set_online_status(string status);
	bool get_self_user(s_user* p_user);
	bool get_user_by_id(string user_id, s_user* p_user);
	bool get_guild_by_id(string guild_id, s_guild* p_guild);
	bool get_guild_channels(string guild_id, vector<s_channel>* p_channel);
	bool get_channel_by_id(string channel_id, s_channel* p_channel);
	void add_gateway_event(const char* eventname, eventhandler hnd);
	void print_event_handlers();
	void search_messages(string guild_id, string content, vector<s_message>* messages, int* total_results, string author_id = "", int offset = 0);
};

string url_encode(const string& value);
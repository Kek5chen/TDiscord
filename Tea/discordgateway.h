#pragma once

#include <websocketpp/client.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <thread>
#include <string>

#include <nlohmann/json.hpp>

#include "structs.h"
#include "cache.h"

#include "discorduserapi.h"

using namespace nlohmann;

typedef websocketpp::client<websocketpp::config::asio_tls_client> socketclient;

void send_heartbeat(socketclient* c, websocketpp::connection_hdl hdl);

class discordclient;

class discordgateway {
private:
	socketclient client;
    string token;
	const char* base_url = "wss://gateway.discord.gg/?v=9&encoding=json";
    void on_message(socketclient* c, websocketpp::connection_hdl hdl, socketclient::message_ptr msg);
    void on_open(socketclient* c, websocketpp::connection_hdl hdl);
public:
    discordcache cache;
    discordclient* dc_client;
    discordgateway(string token, discordclient* dc_client);
    void connect();
    void add_event(const char* eventname, eventhandler hnd);
    int print_event_handlers();
};
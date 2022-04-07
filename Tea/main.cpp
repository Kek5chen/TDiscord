#include <map>
#include <iostream>

#include "discorduserapi.h"
#include "event_structs.h"
#include "logger.h"

discordclient* dc_client;
s_user self_user;


int main(const int argc, char* argv[]) {
	SetConsoleTitleA("TDiscord - Discord User API Implementation");
	logs::init_file_logging();
	logs::welcome();
	if (!argc && strlen(argv[1]) >= 4) {
		logs::error("No auth key supplied in commandline arguments.");
		return 0;
	}

	string str("Authkey received. First four symbols: ");
	str.append(string(argv[1]).substr(0, 4));
	logs::success(str.c_str());

	dc_client = new discordclient(argv[1], false);

	logs::success("Initialization Routine Done!");
    
	while (true);
}


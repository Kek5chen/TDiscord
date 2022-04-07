#pragma once

#include <string>

namespace logs {
	void init_file_logging();

	void error(const char* str);
	void error(const std::string& str);

	void success(const char* str);
	void success(const std::string& str);

	void info(const char* str);
	void info(const std::string& str);

	void debug(const char* str);
	void debug(const std::string& str);

	void welcome();

	void quit();

    const char* welcome_message = u8R"(
████████╗██████╗ ██╗███████╗ ██████╗ ██████╗ ██████╗ ██████╗ 
╚══██╔══╝██╔══██╗██║██╔════╝██╔════╝██╔═══██╗██╔══██╗██╔══██╗
   ██║   ██║  ██║██║███████╗██║     ██║   ██║██████╔╝██║  ██║
   ██║   ██║  ██║██║╚════██║██║     ██║   ██║██╔══██╗██║  ██║
   ██║   ██████╔╝██║███████║╚██████╗╚██████╔╝██║  ██║██████╔╝
   ╚═╝   ╚═════╝ ╚═╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═════╝
-            A Discord API Implementation by kx             -
)";
}
#include "logger.h"
#include <iostream>
#include <termcolor/termcolor.hpp>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ios;

ofstream stream;

void logs::init_file_logging() {
	stream.open("debug_log.txt", ios::out);
}

void logs::error(const char* str) {
	if (stream.is_open()) {
		stream << termcolor::red << "[ERR] " << str << endl << termcolor::reset;
		stream.flush();
	}
	cout << termcolor::red << "[ERR] " << str << endl << termcolor::reset;
}

void logs::error(const std::string& str) {
	error(str.c_str());
}

void logs::success(const char* str) {
	if (stream.is_open()) {
		stream << termcolor::green << "[GOOD] " << str << endl << termcolor::reset;
		stream.flush();
	}
	cout << termcolor::green << "[GOOD] " << str << endl << termcolor::reset;
}

void logs::success(const std::string& str) {
	success(str.c_str());
}

void logs::info(const char* str) {
	if (stream.is_open()) {
		stream << termcolor::yellow << "[INFO] " << str << endl << termcolor::reset;
		stream.flush();
	}
	cout << termcolor::yellow << "[INFO] " << str << endl << termcolor::reset;
}

void logs::info(const std::string& str) {
	info(str.c_str());
}

void logs::debug(const char* str) {
	if (stream.is_open()) {
		stream << termcolor::bright_blue << "[DEBUG] " << str << endl << termcolor::reset;
		stream.flush();
	}
	cout << termcolor::bright_blue << "[DEBUG] " << str << endl << termcolor::reset;
}

void logs::debug(const std::string& str) {
	debug(str.c_str());
}

void logs::welcome() {
	if (stream.is_open()) {
		stream << termcolor::blue << welcome_message << termcolor::reset << endl;
		stream.flush();
	}


	SetConsoleOutputCP(CP_UTF8);
	cout << termcolor::blue << welcome_message << termcolor::reset << endl;
}

void logs::quit() {
	cout << termcolor::blue << "Press any key to exit..." << termcolor::reset << endl;
	//cin.ignore();
	cin.get();
}
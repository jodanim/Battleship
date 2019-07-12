#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <netdb.h>


enum ErrorId{
	WrongIp,NoIfaddrs,NoNameinfo
};
// No es importante
class MessageHandler{
	public:		
		void error(ErrorId id, std::string dataA = "");
		void debug(std::string data, bool lf = true);
		void debug(int data, bool lf = true);
		void debug(char * data, bool lf = true);
	private:
		enum Colors4bit{BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, GRAY};
		std::string singleOut(std::string cnt, std::string unit);
		std::string doubleToString(double data);
		std::string dye(std::string str, Colors4bit c , bool bold = false);
		std::string dye(std::string str, unsigned char c , bool bold = false);
};

#endif /* MESSAGE_HANDLER_HPP */
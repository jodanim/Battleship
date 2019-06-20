#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <netdb.h>

enum MessageId{
	noIPCMessages,
	StartProgram,StartTime,EndTime,FinishProgram
};
enum ErrorId{
	msggetErr,msgctlErr,msgsndErr,msgrcvErr,
	WrongIp,NoIfaddrs,NoNameinfo
};

class MessageHandler{
	public:		
		static void msg(MessageId id, std::string dataA = "", std::string dataB = "", std::string dataC = "");
		static void msg(MessageId id, double dataA, double dataB, std::string dataC = "");
		static void msg(MessageId id, double dataA, std::string dataB);
		static void error(ErrorId id, std::string dataA = "");        	
	private:
		enum Colors4bit{BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, GRAY};
		static std::string singleOut(std::string cnt, std::string unit);
		static std::string doubleToString(double data);
		static std::string dye(std::string str, Colors4bit c , bool bold = false);
		static std::string dye(std::string str, unsigned char c , bool bold = false);
};

#endif /* MESSAGE_HANDLER_HPP */
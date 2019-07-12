#include "../lib/MessageHandler.hpp"

void MessageHandler::error(ErrorId id, std::string dataA){
	std::cout<<std::endl<< dye("ERROR: ",RED,1);
	switch (id){
//-----------------------------------------------------------------------------------------------------------------------------
		case WrongIp:
			std::cout<<dye("Network::stringIptoIntIp ", RED,1)<<dye("\"" + dataA + "\"",YELLOW,1) + dye(" is not a valid IP.",RED,1);
			break;
		case NoIfaddrs:
			std::cout<<dye("Network::getLocalIp ", RED,1)<< dye( strerror(std::stoi( dataA ) ),RED);
			break;
		case NoNameinfo:
			std::cout<<dye("Network::getLocalIp ", RED,1)<< dye( gai_strerror(std::stoi( dataA ) ),RED);
			break;
		default:
			std::cout<< dye("UNHANDLED ERROR: " + std::to_string(id) ,MAGENTA);
			break;
		}
	std::cout<<std::endl<<std::endl;
	exit (EXIT_FAILURE);
}

void MessageHandler::debug(std::string data, bool lf){
	if(lf)std::cout<<std::endl<<std::endl;
	std::cout<<data;
	if(lf)std::cout<<std::endl<<std::endl;
	else std::cout<<", ";
}

void MessageHandler::debug(int data, bool lf){
	if(lf)std::cout<<std::endl<<std::endl;
	std::cout<<data;
	if(lf)std::cout<<std::endl<<std::endl;
	else std::cout<<", ";
}

void MessageHandler::debug(char * data, bool lf){
	if(lf)std::cout<<std::endl<<std::endl;
	std::cout<<data;
	if(lf)std::cout<<std::endl<<std::endl;
	else std::cout<<", ";
}

std::string MessageHandler::singleOut(std::string cnt, std::string unit){
	return (std::stod(cnt) == 1)?unit.substr(0,unit.length()-1):unit;
}

std::string MessageHandler::doubleToString(double data){
	std::string casted = std::to_string(data);
	int l = casted.length()-1;
	while(casted.at(l) == '0' ){
		casted = casted.substr(0,l);
		l--;
	}
	if(casted[l]=='.'){
		casted = casted.substr(0,l);
	}
	return casted;
}

std::string MessageHandler::dye(std::string str, Colors4bit c, bool bold){
	return (std::string)"\033[0" + (bold?";1":"") + ";3" + std::to_string(c) + "m" + str + "\033[0m";
}

std::string MessageHandler::dye(std::string str, unsigned char c, bool bold){
	return (std::string)"\033[0" + (bold?";1":"") + ";38;5;" + std::to_string(c) + "m" + str + "\033[0m";
}

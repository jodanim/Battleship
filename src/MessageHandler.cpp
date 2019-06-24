#include "../lib/MessageHandler.hpp"

void MessageHandler::msg(MessageId id, std::string dataA, std::string dataB, std::string dataC){
	switch (id){
		case noIPCMessages:
			std::cout<<std::endl<< "No ipc messages available" <<std::endl;
			break;	
//-----------------------------------------------------------------------------------------------------------------------------
		case StartProgram:
			std::cout<<std::endl<< dye("Program started, time unit: " + dataA,CYAN) <<std::endl<<std::endl;
			break;
		case StartTime:
			std::cout<<std::endl<<dye("Stage " + dataA +  " started at: " + dataB + " " + singleOut(dataB,dataC),std::stoi(dataA)+54,1) <<std::endl<<std::endl;
			break;
		case EndTime:
			std::cout<<std::endl<<dye("Stage "+ dataA + " ended, duration: " + dataB + " " + singleOut(dataB,dataC),std::stoi(dataA)+54,1) <<std::endl<<std::endl;
			break;
		case FinishProgram:
			std::cout<<std::endl<<dye("Program finished at: " + dataA + " " + singleOut(dataA,dataB),CYAN) <<std::endl<<std::endl;
			break;
		default:
			std::cout<< dye("UNHANDLED MESSAGE: " + std::to_string(id) ,MAGENTA);
			break;
	}
}

void MessageHandler::msg(MessageId id, double dataA, double dataB,std::string dataC){
	msg(id,doubleToString(dataA),doubleToString(dataB),dataC);
}

void MessageHandler::msg(MessageId id, double dataA, std::string dataB){
	msg(id,doubleToString(dataA),dataB);
}

void MessageHandler::error(ErrorId id, std::string dataA){
	std::cout<<std::endl<< dye("ERROR: ",RED,1);
	switch (id){
		case msggetErr:
			std::cout<<dye("Mailbox::Mailbox ", RED,1)<< dye( strerror(std::stoi( dataA ) ),RED);
			break;
		case msgctlErr:
			std::cout<<dye("Mailbox::~Mailbox ", RED,1)<< dye( strerror(std::stoi( dataA ) ),RED);
			break;
		case msgsndErr:
			std::cout<<dye("Mailbox::send", RED,1)<< dye( strerror(std::stoi( dataA ) ),RED);
			break;
		case msgrcvErr:
			std::cout<<dye("Mailbox::receive", RED,1)<< dye( strerror(std::stoi( dataA ) ),RED);
			break;
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
	while((casted.at(l) == '0' )){
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
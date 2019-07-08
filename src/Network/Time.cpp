#include "../../lib/Network/Time.hpp"

Time::Time(TimeUnit unit, bool inform){
	this->unit = unit;
	this->inform = inform;
	if(inform){
		stage = 1;
		initialTime = now();
		MessageHandler messageHandler;
		messageHandler.msg(StartProgram,getTimeUnitName());
	}
}

Time::~Time(){
	if(inform){
		double diff = (now()-initialTime);
		MessageHandler messageHandler;
		messageHandler.msg(FinishProgram,diff,getTimeUnitName());
	}
}

void Time::start(){
	startTime = now();
	if(inform){
		MessageHandler messageHandler;
		messageHandler.msg(StartTime,stage,startTime-initialTime,getTimeUnitName());
	}
}

double Time::end(){
	double diff = now() - startTime;
	if(inform){
		MessageHandler messageHandler;
		messageHandler.msg(EndTime,stage++,diff,getTimeUnitName());
	}
	return diff;
}

double Time::now(){
	std::chrono::nanoseconds now = std::chrono::high_resolution_clock::now().time_since_epoch();
	switch (unit){
		default:
			return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
		case microseconds: 
			return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now).count()*0.001;
		case miliseconds: 	
			return (double)std::chrono::duration_cast<std::chrono::microseconds>(now).count()*0.001;
		case seconds:		
			return (double)std::chrono::duration_cast<std::chrono::milliseconds>(now).count()*0.001;
		case minutes:		
			return (double)std::chrono::duration_cast<std::chrono::seconds>(now).count()/60;
		case hours:			
			return (double)std::chrono::duration_cast<std::chrono::minutes>(now).count()/60;

	}
}

std::string Time::getTimeUnitName(){
	switch (unit){
		default:			return "nanoseconds";
		case microseconds:	return "microseconds";
		case miliseconds: 	return "miliseconds";
		case seconds:		return "seconds";
		case minutes:		return "minutos";
		case hours:			return "horas";
	}
}
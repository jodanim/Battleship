#include "../../lib/Network/FileManager.hpp"

FileManager::FileManager(){}

FileManager::~FileManager(){
	input.close();
}

void FileManager::open(const char * path, bool read){
	std::ios_base::openmode openmode= (read)?std::fstream::in:std::fstream::out;
	input.open(path,openmode);
}

 size_t FileManager::getLine(char * buffer){
	buffer[0] = '\0';
	std::string line;
	getline(input,line);
	size_t len = line.size();
	std::strncpy(buffer,line.c_str(),len);
	buffer[len] = '\0';
	return len;
}

void FileManager::close(){
	input.close();
}
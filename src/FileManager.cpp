#include "../lib/FileManager.hpp"

FileManager::FileManager(){}

FileManager::~FileManager(){
	input.close();
}

void FileManager::open(std::string path){
	input.open(path);
}

std::string FileManager::getLine(){
	std::string line;
	getline(input,line);
	return line;
}

void FileManager::close(){
	input.close();
}
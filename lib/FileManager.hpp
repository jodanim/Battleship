#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <string>
#include <fstream>
#include <iostream>

class FileManager{
	public:
		FileManager();
		~FileManager();
		void open(std::string path);
		std::string getLine();
		void close();
	private:
		std::ifstream input;
};

#endif /* FILEMANAGER_HPP */
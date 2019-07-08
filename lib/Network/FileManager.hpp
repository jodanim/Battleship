#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>
#include <cstring>

class FileManager{
	public:
		FileManager();
		~FileManager();
		void open(const char * path, bool read = true);
		size_t getLine(char * buffer);
		void close();
	private:
		std::ifstream input;
};

#endif /* FILEMANAGER_HPP */
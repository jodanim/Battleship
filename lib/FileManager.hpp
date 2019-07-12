#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>
#include <cstring>

class FileManager{
	public:
		
		/**
		 * Destructor
		 * 
		 * EFE: Closes the open file alocated on the ifstream input
		 */
		~FileManager();
		
		/**
		 * open
		 * 
		 * EFE: Opens the given file in write mode or read mode
		 * REQ: path: The file path
		 * 		read: S flag that indicates if the open mode is read or write default(true)
		 */
		void open(const char * path, bool read = true);

		/**
		 * getLine
		 * 
		 * EFE: Reads a line from the file.
		 * REQ: buffer: A buffer where to store the line.
		 * RET: The line stored on buffer and its size as return value.
		 */
		size_t getLine(char * buffer);
		
		/**
		 * close
		 * 
		 * EFE: Closes the file.
		 */
		void close();
		
	private:

		// The file
		std::ifstream input;
};

#endif /* FILEMANAGER_HPP */
#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include "MessageHandler.hpp"
#include <cstdio>

class Translator{
	public:
		unsigned int constCharIptoIntIp(const char * ip);	
		void intIptoCharArrayIp(unsigned int ip, char * array);	
		unsigned int byteArrayToNumber(const unsigned char * array, int bytes);
};

#endif /* TRANSLATOR_HPP */

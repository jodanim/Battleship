#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include "MessageHandler.hpp"
#include <cstdio>

class Translator{
	public:
		
		/**
		 * constCharIptoIntIp
		 * 
		 * EFE: Translate an ip on its normal display to its unsigned int value.
		 * REQ: ip: A char array with the ip to trasnlate.
		 * RET: The unsigned int value of the ip
		 */
		unsigned int constCharIptoIntIp(const char * ip);
		
		/**
		 * intIptoCharArrayIp
		 * 
		 * EFE: Translate the unsigned int value of an ip to its normal display.
		 * REQ: ip: the unsigned int value of the ip
		 * 		array: A char array where to store the ip.
		 * RET: the ip on its normal display on the parameter "array"
		 */
		void intIptoCharArrayIp(unsigned int ip, char * array);	
		std::string intIptoStringIp(unsigned int ip);	

		/**
		 * byteArrayToNumber
		 * 
		 * EFE: Translate a byte array of length n to its positive numberic value.
		 * REQ: array: A char array with the bytes to be translated.
		 * 		bytes: the number of bytes on the array, max 4
		 * RET: the numeric value
		 */
		unsigned int byteArrayToNumber(const unsigned char * array, int bytes);
};

#endif /* TRANSLATOR_HPP */

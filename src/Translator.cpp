#include "../lib/Translator.hpp"

unsigned int Translator::constCharIptoIntIp(const char * ip){
    MessageHandler messageHandler;
	int l = strlen(ip);
	// Operationless comprobations, size limits between 7 and 15, dots count = 3 and dots not at start or end.
	if( (l<7) || (l>15) || (ip[0]=='.') || (ip[l-1]=='.') )
		messageHandler.error(WrongIp, ip);

	int dots = 0;
	for(int i=0; i<l; i++)// Counting dots.
		if(ip[i]=='.') dots++;
	if(dots != 3)
		messageHandler.error(WrongIp, ip);
	unsigned int intIp = 0;
	int groupNum = 0;
	for(int i=0, j; i<l; i=j+1){// Iterating over each octet of the ip.
		bool notZero=false, zero=false;
		for(j=i; (ip[j]!='.') && (j<l); j++){// Iterating over each char of the octet.
			// char by char comprobations, is a number and there arent left zeros.
			if( (ip[j]<'0') || (ip[j]>'9') || (zero && !notZero) )
				messageHandler.error(WrongIp, ip);
			if(ip[j]=='0') zero = true;
			else notZero = true;
		}
		// Getting the octet.
		char group[4];
		strncpy(group,ip+i,j-i);
		group[j-i]='\0';
		int octet = std::atoi(group);
		// Final comprobations is an 8 bites number, there arent consecutive points.
		if( !(octet <= 255) || ( (ip[j]=='.') && (ip[j-1]=='.') ) ) 
			messageHandler.error(WrongIp, ip);
		// Adding the octet to the result.
		octet<<=8*groupNum++; 
		intIp += octet;
	}
	return intIp;
}

void Translator::intIptoCharArrayIp(unsigned int ip, char * array){
	char first[5], second[5], third[5], fourth[4];
	int l1,l2,l3,l4;
	sprintf (first, "%d.",ip    &0xFF);
	sprintf (second,"%d.",ip>>8 &0xFF);
	sprintf (third, "%d.",ip>>16&0xFF);
	sprintf (fourth,"%d" ,ip>>24     );
	l1 = strlen(first);
	l2 = strlen(second);
	l3 = strlen(third);
	l4 = strlen(fourth);
	strncpy(array,first,l1);
	strncpy(array+l1,second,l2);
	strncpy(array+l1+l2,third,l3);
	strncpy(array+l1+l2+l3,fourth,l4);
}

//-----------------------------------------------------------------------------------------------------------------------------

unsigned int Translator::byteArrayToNumber(const unsigned char * array, int bytes){
	unsigned int result = 0;
	for(int i = 0; i < bytes; i++){
		result += array[i]<<(8*i);
	}
    return result;
}

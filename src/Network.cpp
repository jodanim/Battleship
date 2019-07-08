#include "../lib/Network.hpp"
#include <iostream>

Network::Network(int port, double reliability){
	srand(time(NULL));
	this->reliability = (reliability<0)? 0 : (reliability>1)? 1 : reliability;
	packetsSent = 0;
    sendingPacket = false;
	packetAvailable = false;
	exit = false;
	char buffer[IP_MAX_SIZE];
	getLocalIp(buffer);
	ip = constCharIptoIntIp(buffer); 
	
	socket.Bind(port);
	this->port = port;
	receiver = std::thread(&Network::checkPacketAvailable, this);
}

Network::~Network(){
	char buffer[IP_MAX_SIZE];
	intIptoCharArrayIp(this->ip, buffer);
	send(buffer,this->port,"end");
	exit = true;
	receiver.join();
}

void Network::send(const char * ip, int port, const char * data){
	writeHandler();
	std::thread(&Network::sendDone, this).detach();
	if(rand()%100+1 > reliability*100){
		std::cout<<"failed";
		return;
	}
	Packet packet;
	int len = (MAXDATASIZE-1>strlen(data))? strlen(data): MAXDATASIZE-1;
	strncpy(packet.data, data, len);
	packet.data[len] = '\0';

	PacketHeader header;
	header.from = this->ip;
	header.to = constCharIptoIntIp(ip);
	header.dataSize = strlen(packet.data)+1;
	packet.header = header;

	unsigned char buffer[MAXWIRESIZE];
	memcpy(buffer,(const unsigned char*)&packet,MAXWIRESIZE);

	byteArrayToPacket(buffer);

	socket.Write(buffer,MAXWIRESIZE,ip,port);
}

void Network::writeHandler(){
	while(sendingPacket);
	sendingPacket = true;
}

void* Network::sendDone(){
	sendingPacket = false;
	packetsSent++;
	pthread_exit(NULL);
}

void* Network::checkPacketAvailable(){
	while(!exit){
		unsigned char buffer[MAXWIRESIZE];
		socket.Read(buffer,MAXWIRESIZE);
		Packet packet = byteArrayToPacket(buffer);
		receivedPackets.push_back(packet);
		packetAvailable = true;
	}
	pthread_exit(EXIT_SUCCESS);
}

PacketHeader Network::receive(char * data){
	readHandler();
	Packet received = receivedPackets.front();
    receivedPackets.erase(receivedPackets.begin());
	strncpy(data,received.data,received.header.dataSize);
	return received.header;
}

void Network::readHandler(){
	if(receivedPackets.size()==0)packetAvailable=false;
	while(!packetAvailable);
}

//-----------------------------------------------------------------------------------------------------------------------------

unsigned int Network::constCharIptoIntIp(const char * ip){
	int l = strlen(ip);
	// Operationless comprobations, size limits between 7 and 15, dots count = 3 and dots not at start or end.
	if( (l<7) || (l>15) || (ip[0]=='.') || (ip[l-1]=='.') )
		MessageHandler::error(WrongIp, ip);

	int dots = 0;
	for(int i=0; i<l; i++)// Counting dots.
		if(ip[i]=='.') dots++;
	if(dots != 3)
		MessageHandler::error(WrongIp, ip);
	unsigned int intIp = 0;
	int groupNum = 0;
	for(int i=0, j; i<l; i=j+1){// Iterating over each octet of the ip.
		bool notZero=false, zero=false;
		for(j=i; (ip[j]!='.') && (j<l); j++){// Iterating over each char of the octet.
			// char by char comprobations, is a number and there arent left zeros.
			if( (ip[j]<'0') || (ip[j]>'9') || (zero && !notZero) )
				MessageHandler::error(WrongIp, ip);
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
			MessageHandler::error(WrongIp, ip);
		// Adding the octet to the result.
		octet<<=8*groupNum++; 
		intIp += octet;
	}
	return intIp;
}

void Network::intIptoCharArrayIp(unsigned int ip, char * array){
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

Packet Network::byteArrayToPacket(const unsigned char * bytes){
    Packet packet;
    packet.header.to = byteArrayToInt(bytes);
    packet.header.from = byteArrayToInt(bytes+4);
    packet.header.dataSize = byteArrayToInt(bytes+8);
	strncpy(packet.data,(const char*)bytes+12,packet.header.dataSize);
    return packet;
}

unsigned int Network::byteArrayToInt(const unsigned char * array){
    unsigned int first = array[0];
    unsigned int second = array[1]<<8;
    unsigned int third = array[2]<<16;
    unsigned int fourth = array[3]<<24;
    return first+second+third+fourth;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Network::getLocalIp(char * ip, int family){
	// Get the default network interface.
	char interface[NETWORK_INTERFACE_MAX_LENGTH];
	getDefaultInterface(interface);
	char host[NI_MAXHOST];
	// Get all the ifaddrs.
	struct ifaddrs *ifaddr;
	if(getifaddrs(&ifaddr) != 0) 
		MessageHandler::error(NoIfaddrs, std::to_string(errno));

	//Searches on all the ifaddrs the one who matches the interface.
	for(struct ifaddrs*it=ifaddr; it!=NULL; it=it->ifa_next){
		if(it->ifa_addr==NULL) continue;
		if(strcmp(it->ifa_name, interface)==0){
			if (it->ifa_addr->sa_family==family){
				size_t size = (family==AF_INET)? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
				if(getnameinfo(it->ifa_addr, size, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
					MessageHandler::error(NoNameinfo, std::to_string(errno));
			}
		}
	}
	// free mem.
	freeifaddrs(ifaddr);
	// return ip.
	strncpy(ip,host,IP_MAX_SIZE);
}

void Network::getDefaultInterface(char * interface){
	fileManager.open("/proc/net/route");
	char buffer[PROC_NET_ROUTE_LINE_SIZE];
	int pos;
	do{// search the default interface on every line of the file.
		fileManager.getLine(buffer);
		char * c = std::strchr(buffer,'\t');
		pos = c-buffer+1;
		strncpy(interface,buffer,pos);
		interface[pos-1] = '\0';
	// The deafault interface is the one whose destination is 00000000.
	}while(strncmp(buffer+pos,"00000000",8)!=0);
	fileManager.close();
}
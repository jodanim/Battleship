#include "../lib/Network.hpp"
#include <iostream>

Network::Network(int port, double reliability){
	srand(time(NULL));
	this->reliability = (reliability<0)? 0 : (reliability>1)? 1 : reliability;
	packetAvailable = false;
	exit = false;
	char buffer[IP_MAX_SIZE];
	getLocalIp(buffer);
	ip = translator.constCharIptoIntIp(buffer);
	socket.Bind(port);
	this->port = port;
	receiver = std::thread(&Network::checkPacketAvailable, this);
}

Network::~Network(){
	exit = true;
	reliability = 1;
	PacketHeader header(ip,port);
	send(header,"");
	receiver.join();
}

// ----------------------------------------------------------------------------------------------------------------------

void Network::sendMessage(PacketHeader header, const char * message){
	int messageLength = strlen(message);
	header.messageSize = messageLength;
	int processedBytes = 0;
	int i = 0;
	while(processedBytes<messageLength){
		int dataLength = ((MAX_DATA_SIZE - 1) < (messageLength-processedBytes))?MAX_DATA_SIZE-1:messageLength-processedBytes;
		header.id = i++;
		char buffer[MAX_DATA_SIZE];
		strncpy(buffer,message+processedBytes,dataLength);
		processedBytes += dataLength;
		buffer[dataLength] = '\0';
		send(header, buffer);
	}
}

PacketHeader Network::receiveMessage(char * message){
	PacketHeader header = receive(message);
	int processedBytes = header.dataSize;
	int messageSize = header.messageSize;
	while(messageSize>processedBytes){
		char buffer[MAX_DATA_SIZE];
		header = receive(buffer);
		strncpy(message+processedBytes,buffer,header.dataSize);
		processedBytes += header.dataSize;
	}
	message[messageSize] = '\0';
	return header;
}

// ----------------------------------------------------------------------------------------------------------------------

void Network::send(PacketHeader header, const char * data){
	writeHandler();
	std::thread(&Network::sendDone, this).detach();
	if((header.dataSize = rand()%100+1) > reliability*100){
		std::cout<<header.dataSize<<">"<<reliability<<": FAILED "<<header.id<<"\n";
		return;
	}
	Packet packet;
	int len = (MAX_DATA_SIZE-1>strlen(data))? strlen(data): MAX_DATA_SIZE-1;
	strncpy(packet.data, data, len);
	packet.data[len] = '\0';

	header.from = ip;
	header.portFrom = port;
	header.dataSize = strlen(packet.data);
	packet.header = header;

	unsigned char buffer[MAX_WIRE_SIZE];
	memcpy(buffer,(const unsigned char*)&packet,MAX_WIRE_SIZE);
	socket.Write(buffer,MAX_WIRE_SIZE,header.to,header.portTo);
}

void Network::writeHandler(){
	sendingPacket.lock();
}

void* Network::sendDone(){
	sendingPacket.unlock();
	pthread_exit(NULL);
}

void* Network::checkPacketAvailable(){
	while(!exit){
		unsigned char buffer[MAX_WIRE_SIZE];
		socket.Read(buffer,MAX_WIRE_SIZE);
		Packet packet = byteArrayToPacket(buffer);
		receiving.lock();
		receivedPackets.push_back(packet);
		receiving.unlock();
		packetAvailable = true;
	}
	pthread_exit(EXIT_SUCCESS);
}

PacketHeader Network::receive(char * data){
	readHandler();
	receiving.lock();
	Packet received = receivedPackets.front();
    receivedPackets.erase(receivedPackets.begin());
	packetAvailable = !receivedPackets.empty();
	receiving.unlock();
	strncpy(data,received.data,received.header.dataSize);
	return received.header;
}

void Network::readHandler(){
	while(!packetAvailable);
}

// ----------------------------------------------------------------------------------------------------------------------

Packet Network::byteArrayToPacket(const unsigned char * bytes){
    Packet packet;
    packet.header.messageSize = translator.byteArrayToNumber(bytes,4);
	packet.header.from = translator.byteArrayToNumber(bytes+4,4);
	packet.header.to = translator.byteArrayToNumber(bytes+8,4);
	packet.header.portFrom = translator.byteArrayToNumber(bytes+12,2);
	packet.header.portTo = translator.byteArrayToNumber(bytes+14,2);
	packet.header.id = translator.byteArrayToNumber(bytes+16,2);
	packet.header.dataSize = translator.byteArrayToNumber(bytes+18,1);
	packet.header.FrameNum = translator.byteArrayToNumber(bytes+19,1);
	strncpy(packet.data,(const char*)bytes+sizeof(PacketHeader),packet.header.dataSize);
    return packet;
}

// ----------------------------------------------------------------------------------------------------------------------

void Network::getLocalIp(char * ip, int family){
	MessageHandler messageHandler;
	// Get the default network interface.
	char interface[NETWORK_INTERFACE_MAX_LENGTH];
	getDefaultInterface(interface);
	char host[NI_MAXHOST];
	// Get all the ifaddrs.
	struct ifaddrs *ifaddr;
	if(getifaddrs(&ifaddr) != 0) 
		messageHandler.error(NoIfaddrs, std::to_string(errno));

	//Searches on all the ifaddrs the one who matches the interface.
	for(struct ifaddrs*it=ifaddr; it!=NULL; it=it->ifa_next){
		if(it->ifa_addr==NULL) continue;
		if(strcmp(it->ifa_name, interface)==0){
			if (it->ifa_addr->sa_family==family){
				size_t size = (family==AF_INET)? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
				if(getnameinfo(it->ifa_addr, size, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
					messageHandler.error(NoNameinfo, std::to_string(errno));
			}
		}
	}
	// free mem.
	freeifaddrs(ifaddr);
	// return ip.
	strncpy(ip,host,IP_MAX_SIZE);
}

void Network::getDefaultInterface(char * interface){
	FileManager fileManager;
	fileManager.open("/proc/net/route");
	char buffer[PROC_NET_ROUTE_LINE_SIZE];
	int pos;
	do{// search the default interface on every line of the file.
		fileManager.getLine(buffer);
		if(strlen(buffer)!=0){
			char * c = std::strchr(buffer,'\t');
			pos = c-buffer+1;
			strncpy(interface,buffer,pos);
			interface[pos-1] = '\0';
		}
	// The deafault interface is the one whose destination is 00000000.
	}while(strncmp(buffer+pos,"00000000",8)!=0&&strlen(buffer)!=0);
	fileManager.close();
}
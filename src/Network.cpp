#include "../lib/Network.hpp"
#include <iostream>

Network::Network(int port, double reliability){
	srand(time(NULL));
	this->reliability = (reliability<0)? 0 : (reliability>1)? 1 : reliability;
	exit = false;
	std::string localIp = getLocalIp();	
	ip = translator.constCharIptoIntIp(localIp.c_str());
	socket.Bind(port);
	this->port = port;
	receiver = std::thread(&Network::checkPacketAvailable, this);
}

Network::~Network(){
	exit = true;
	reliability = 1;
	socket.Write((const unsigned char*)"end",3,ip,port);
	receiver.join();
}

// ----------------------------------------------------------------------------------------------------------------------

void Network::sendMessage(PacketHeader header, const char * message){
	if(header.to == 0)header.to = ip;
	failcounter = 0;
	connectionLost = 0;
	header.messageSize = strlen(message);
	int processedBytes = 0;
	int i = 0;
	while(processedBytes<header.messageSize && connectionLost<CONECTION_RETRIES){
		int dataLength = ((MAX_DATA_SIZE - 1) < (header.messageSize-processedBytes))?MAX_DATA_SIZE-1:header.messageSize-processedBytes;
		header.frameNum = i++%2;
		char buffer[MAX_DATA_SIZE];
		strncpy(buffer,message+processedBytes,dataLength);
		processedBytes += dataLength;
		buffer[dataLength] = '\0';
		send(header, buffer);
	}
	if(connectionLost==CONECTION_RETRIES){
		std::string receiverIp = translator.intIptoStringIp(header.to);
		std::cout<<"\033[31mConnection with \033[33m"<<receiverIp<<":"<<header.portTo<<"\033[31m Lost.\033[0m\n";	
	}
}

void Network::send(PacketHeader header, const char * data){
	writeHandler();
	std::thread(&Network::sendDone, this).detach();

	Packet * packet = new Packet;
	int len = (MAX_DATA_SIZE-1>strlen(data))? strlen(data): MAX_DATA_SIZE-1;
	strncpy(packet->data, data, len);
	packet->data[len] = '\0';
	header.from = ip;
	header.portFrom = port;
	header.dataSize = len;
	packet->header = header;

	unsigned char buffer[MAX_WIRE_SIZE];
	memcpy(buffer,(const unsigned char*)packet,MAX_WIRE_SIZE);

	bool acknowledged = false;
	while(!acknowledged&&connectionLost<CONECTION_RETRIES){
		bool timeout = false;
		acknowledged = false;
		if((rand()%100+1) > reliability*100){
			std::cout<<"\033[1A\033[31mSENDING FAILED "<< failcounter++<<" TIMES.\033[0m\n";
			timeout = true;
		}else{
			socket.Write(buffer,MAX_WIRE_SIZE,header.to,header.portTo);
		}
		if(!timeout){
			std::thread timeThread(&Network::runClock,this, std::ref(timeout),std::ref(acknowledged));
			receiveACK(header,timeout,acknowledged);
			timeThread.join();
		}
	}
	delete packet;
}

void Network::writeHandler(){
	sendingPacket.lock();
}

void Network::runClock(bool &timeout, bool &acknowledged){
	for(int i = 0; i < 2500; i++){
		usleep(1);
		if(acknowledged)pthread_exit(NULL);
	}
	
	acknowledging.lock();
	if(!acknowledged){
		timeout = true;
		connectionLost++;
		std::cout<<connectionLost<<": Timeout\n";
	}
	acknowledging.unlock();
	pthread_exit(NULL);
}

void Network::receiveACK(PacketHeader header, bool &timeout, bool &acknowledged){
	while(receivedPackets[header.to][header.portTo].empty() && !timeout);
	acknowledging.lock();
	if(!timeout){
		for (std::vector<Packet>::iterator it = receivedPackets[header.to][header.portTo].begin() ; it != receivedPackets[header.to][header.portTo].end(); ++it){
    		if(strncmp(it->data,"ACK",MAX_DATA_SIZE)==0){
				if(it->header.frameNum == header.frameNum){
					acknowledged = true;
				}
				receivedPackets[header.to][header.portTo].erase(it);
				break;
			}
		}
	}
	acknowledging.unlock();
}

void Network::sendDone(){
	sendingPacket.unlock();
	pthread_exit(NULL);
}

// ----------------------------------------------------------------------------------------------------------------------

std::string Network::receiveMessage(PacketHeader header){
	PacketHeader fliped(header.to,header.portTo, true);
	if(fliped.from == 0)fliped.from = ip;
	std::string message = "";
	do{
		message += receive(fliped);
	}while(fliped.messageSize>message.size());
	return message;
}

std::string Network::receive(PacketHeader &header){
	readHandler(header);
	receiving.lock();
	Packet received;
	for (std::vector<Packet>::iterator it = receivedPackets[header.from][header.portFrom].begin() ; it != receivedPackets[header.from][header.portFrom].end(); ++it){
		if(strncmp(it->data,"ACK",MAX_DATA_SIZE)!=0){
			received = receivedPackets[header.from][header.portFrom].front();
			receivedPackets[header.from][header.portFrom].erase(it);
			break;
		}
	}
	receiving.unlock();
	header = received.header;
	return std::string(received.data,received.header.dataSize);
}

void Network::readHandler(PacketHeader header){
	while(receivedPackets[header.from][header.portFrom].empty());
}

// ----------------------------------------------------------------------------------------------------------------------

void Network::checkPacketAvailable(){
	while(!exit){
		unsigned char buffer[MAX_WIRE_SIZE];
		socket.Read(buffer,MAX_WIRE_SIZE);
		Packet packet = byteArrayToPacket(buffer);
		receiving.lock();
		if(strncmp(packet.data,"ACK",MAX_DATA_SIZE) != 0){
			sendACK(packet.header);
		}
		receivedPackets[packet.header.from][packet.header.portFrom].push_back(packet);		
		receiving.unlock();
	}
	pthread_exit(EXIT_SUCCESS);
}

void Network::sendACK(PacketHeader header){
	PacketHeader ack(header.from,header.portFrom);
	Packet packet;
	strncpy(packet.data, "ACK", 3);
	ack.from = ip;
	ack.portFrom = port;
	ack.dataSize = 3;
	ack.frameNum = header.frameNum;
	packet.header = ack;
	unsigned char buffer[MAX_WIRE_SIZE];
	memcpy(buffer,(const unsigned char*)&packet,MAX_WIRE_SIZE);
	socket.Write(buffer,MAX_WIRE_SIZE,packet.header.to,packet.header.portTo);
}
// ----------------------------------------------------------------------------------------------------------------------

Packet Network::byteArrayToPacket(const unsigned char * bytes){
    Packet packet;
    packet.header.messageSize = translator.byteArrayToNumber(bytes,4);
	packet.header.from = translator.byteArrayToNumber(bytes+4,4);
	packet.header.to = translator.byteArrayToNumber(bytes+8,4);
	packet.header.portFrom = translator.byteArrayToNumber(bytes+12,2);
	packet.header.portTo = translator.byteArrayToNumber(bytes+14,2);
	packet.header.dataSize = translator.byteArrayToNumber(bytes+16,2);
	packet.header.frameNum = translator.byteArrayToNumber(bytes+18,2);
	strncpy(packet.data,(const char*)bytes+sizeof(PacketHeader),packet.header.dataSize);
    return packet;
}

// ----------------------------------------------------------------------------------------------------------------------

std::string Network::getLocalIp(int family){
	MessageHandler messageHandler;
	// Get the default network interface.
	std::string interface =	getDefaultInterface();
	char host[NI_MAXHOST];
	// Get all the ifaddrs.
	struct ifaddrs *ifaddr;
	if(getifaddrs(&ifaddr) != 0) 
		messageHandler.error(NoIfaddrs, std::to_string(errno));

	//Searches on all the ifaddrs the one who matches the interface.
	for(struct ifaddrs*it=ifaddr; it!=NULL; it=it->ifa_next){
		if(it->ifa_addr==NULL) continue;
		if(strcmp(it->ifa_name, interface.c_str())==0){
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
	return std::string(host);
}

std::string Network::getDefaultInterface(){
	FileManager fileManager;
	fileManager.open("/proc/net/route");
	char buffer[PROC_NET_ROUTE_LINE_SIZE];
	int pos;
	std::string interface;
	do{// search the default interface on every line of the file.
		fileManager.getLine(buffer);
		if(strlen(buffer)!=0){
			char * c = std::strchr(buffer,'\t');
			pos = c-buffer;
			interface = std::string(buffer,pos);
		}
	// The deafault interface is the one whose destination is 00000000.
	}while(strncmp(buffer+pos,"00000000",8)!=0&&strlen(buffer)!=0);
	fileManager.close();
	return interface;
}

// ----------------------------------------------------------------------------------------------------------------------
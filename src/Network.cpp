#include "../lib/Network.hpp"
#include <iostream>

Network::Network(int port, double reliability){
	srand(time(NULL));
	this->reliability = (reliability<0)? 0 : (reliability>1)? 1 : reliability;
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
	socket.Write((const unsigned char*)"end",4,ip,port);
	receiver.join();
}

// ----------------------------------------------------------------------------------------------------------------------
void Network::sendACK(PacketHeader header){
	PacketHeader ack(header.from,header.portFrom);
	ack.from = header.to;
	ack.portFrom = header.portTo;
	ack.frameNum = header.frameNum;
	ack.id = header.id;
	ack.messageSize = 0;
	ack.dataSize = MAX_WIRE_SIZE;
	Packet empty;
	empty.header = ack;
	unsigned char buffer[MAX_WIRE_SIZE];
	memcpy(buffer,(const unsigned char*)&empty,MAX_WIRE_SIZE);
	socket.Write(buffer,MAX_WIRE_SIZE,ack.to,ack.portTo);
	// //std::cout<<"Sending ACK to: " << ack.to<<":"<<ack.portTo;
}

void Network::receiveACK(PacketHeader header, bool &timeout, bool &acknowledged){
	//std::cout<<"Receiving ACK from: " << header.to << ":" << header.portTo <<"\n";
	while(receivedACK[header.to][header.portTo] != header.frameNum && !timeout);
	//std::cout<<(int)receivedACK[header.to][header.portTo]<<":"<<(int)header.frameNum<<"\n";
	if(!timeout){
		acknowledged = true;
		//std::cout<<"acknowledged: ["	 << (int)receivedACK[header.to][header.portTo] <<":"<<(int)header.frameNum<<"]\n"	;
		acknowledge.unlock();
	}
}

void Network::runClock(bool &timeout, bool &acknowledged){
	sleep(1);
	if(!acknowledged){
		timeout = true;
		connectionLost++;
		acknowledge.unlock();
	}
	pthread_exit(NULL);
}
// ----------------------------------------------------------------------------------------------------------------------

void Network::sendMessage(PacketHeader header, const char * message){
	connectionLost = 0;
	//std::cout<<"sending message to: "<<header.to<<":"<<header.portTo<<"\n";
	//std::cout<<"Message: "<<message<<"\n\n";
	receivedACK[header.to][header.portTo] = -1;
	int messageLength = strlen(message);
	header.messageSize = messageLength;
	int processedBytes = 0;
	int i = 0;
	while(processedBytes<messageLength && connectionLost<CONECTION_RETRIES){
		int dataLength = ((MAX_DATA_SIZE - 1) < (messageLength-processedBytes))?MAX_DATA_SIZE-1:messageLength-processedBytes;
		header.frameNum = i%2;
		header.id = i++;
		char buffer[MAX_DATA_SIZE];
		strncpy(buffer,message+processedBytes,dataLength);
		processedBytes += dataLength;
		buffer[dataLength] = '\0';
		send(header, buffer);
	}
	if(connectionLost==CONECTION_RETRIES){
		std::cout<<"\033[31mConnection Lost.\033[0m\n";	
	}
	//std::cout<<"End transmission\n\n";
}

PacketHeader Network::receiveMessage(char * message){
	//std::cout<<"Receiving message from anywhere\n\n";
	PacketHeader header;
	//std::cout<<"\033[32mstart first receive\033[0m\n";
	header = receive(message,header);
	//std::cout<<"\033[32mend first receive\033[0m\n\n";
	int processedBytes = header.dataSize;
	int messageSize = header.messageSize;
	while(messageSize>processedBytes){
		char buffer[MAX_DATA_SIZE];
		//std::cout<<"\033[32mStart receive "<<header.id+1<<"\033[0m\n";
		header = receive(buffer,header);
		//std::cout<<"\033[32mEnd receive "<<header.id<<"\033[0m\n\n";
		strncpy(message+processedBytes,buffer,header.dataSize);
		processedBytes += header.dataSize;
	}
	message[messageSize] = '\0';
	//std::cout<<"Transmission ended.\n\n";
	incommingMessage[header.from].erase(header.portFrom);
	return header;
}

// ----------------------------------------------------------------------------------------------------------------------

void Network::send(PacketHeader header, const char * data){
	writeHandler();
	std::thread(&Network::sendDone, this).detach();

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

	bool acknowledged = false;
	while(!acknowledged&&connectionLost<CONECTION_RETRIES){
		//std::cout<<"Sending packet: "<<packet.header.id<<std::endl;
		if((rand()%100+1) > reliability*100){
			//std::cout<<"\033[31mFAILED "<<header.id<<"\033[0m\n";
		}else{
			socket.Write(buffer,MAX_WIRE_SIZE,header.to,header.portTo);
		}

		bool timeout = false;
		acknowledged = false;
		std::thread(&Network::runClock,this, std::ref(timeout),std::ref(acknowledged)).detach();
		receiveACK(header,timeout,acknowledged);
		acknowledge.lock();
		//std::cout<<"Timeout: "<<timeout<<std::endl<<std::endl;
	}
}

void Network::writeHandler(){
	sendingPacket.lock();
}

void Network::sendDone(){
	sendingPacket.unlock();
	pthread_exit(NULL);
}

// ----------------------------------------------------------------------------------------------------------------------

void Network::checkPacketAvailable(){
	while(!exit){
		unsigned char buffer[MAX_WIRE_SIZE];
		socket.Read(buffer,MAX_WIRE_SIZE);
		Packet packet = byteArrayToPacket(buffer);
		receiving.lock();
		if(packet.header.dataSize == MAX_WIRE_SIZE){
			receivedACK[packet.header.from][packet.header.portFrom] = packet.header.frameNum;
		}else{
			std::map<unsigned int, std::map<unsigned short, bool>>::iterator it;
			it = incommingMessage.find(packet.header.from);
			if(it == incommingMessage.end()){
				//std::cout<<"\033[31mInserting into first packet A\n";
				firstPacket.push_back(packet);
				incommingMessage[packet.header.from][packet.header.portFrom] = true;
			}else{
				if(it->second.find(packet.header.portFrom) == it->second.end()){
					//std::cout<<"\033[31mInserting into first packet B\n";
					firstPacket.push_back(packet);
					incommingMessage[packet.header.from][packet.header.portFrom] = true;
				}else{
					receivedPackets[packet.header.from][packet.header.portFrom].push_back(packet);
				}
			}
			sendACK(packet.header);
		}
		receiving.unlock();
	}
	pthread_exit(EXIT_SUCCESS);
}

PacketHeader Network::receive(char * data, PacketHeader header){
	//std::cout<<"\033[33mWaiting receive\033[0m\n";
	readHandler(header);
	//std::cout<<"\033[33mEntered receive\033[0m\n";
	receiving.lock();
	Packet received;
	//std::cout<<"Frame: "<<(int)header.frameNum<<"\n";
	if(header.frameNum == -1){
		received = firstPacket.front();
		firstPacket.erase(firstPacket.begin());
		//std::cout<<"Receiving packet: "<<received.header.id<<"\n";
		//std::cout<<"Total Size: "<<received.header.messageSize<<", packet size: "<<(int)received.header.dataSize<<"\n";
	}else{
		received = receivedPackets[header.from][header.portFrom].front();
    	receivedPackets[header.from][header.portFrom].erase(receivedPackets[header.from][header.portFrom].begin());
		//std::cout<<"Receiving packet: "<<received.header.id<<"\n";
		//std::cout<<"Total Size: "<<received.header.messageSize<<", packet size: "<<(int)received.header.dataSize<<"\n";
	}

	receiving.unlock();
	strncpy(data,received.data,received.header.dataSize);
	return received.header;
}

void Network::readHandler(PacketHeader header){
	if(header.frameNum==-1){
		//std::cout<<"There is a packet available?\n";
		while(firstPacket.empty());
	}else{
		//std::cout<<"There is the next packet available from "<<header.from<<":"<<header.portFrom<<"?\n";
		while(receivedPackets[header.from][header.portFrom].empty());
	}
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
	packet.header.frameNum = translator.byteArrayToNumber(bytes+19,1);
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

// ----------------------------------------------------------------------------------------------------------------------
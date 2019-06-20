#include "../lib/Network.hpp"

Network::Network(int port,double reliability){
	this->reliability = (reliability<0)? 0 : (reliability>1)? 1 : reliability;
    sendingPacket = false;
	ip = stringIptoIntIp(getLocalIp()); 
	socket.Bind(port);
	std::cout<<ip<<std::endl;
	std::thread(&Network::checkPacketAvailable, this).detach();
	sleep(2);
	std::cout<<ip<<std::endl;
}

Network::~Network(){

}

void Network::send(std::string ip, std::string data){
	PacketHeader hdr;
	hdr.from = this->ip;
	hdr.to = stringIptoIntIp(ip);

	if(rand()%100+1 > reliability*100){
		return;
	}

	if(data.size()>MAXDATASIZE){
		data.resize(MAXDATASIZE);
	}
	hdr.size = data.size();
}



Packet Network::receive(){
	Packet received = receivedPackets.front();
    receivedPackets.erase(receivedPackets.begin());
	return received;
}

void Network::checkPacketAvailable(){
	std::cout<< "wiiiiiiiiiii" ;
	ip = 0;
}











std::string Network::intIptoStringIp(unsigned int ip){
	std::string stringIp = "";
	stringIp += std::to_string(ip>>24)+".";
	stringIp += std::to_string(ip>>16&0xFF)+".";
	stringIp += std::to_string(ip>>8&0xFF)+".";
	stringIp += std::to_string(ip&0xFF);
	return stringIp;
}

unsigned int Network::stringIptoIntIp(std::string ip){
	int l = ip.length();
	// Operationless comprobations, size limits between 7 and 15, dots count = 3 and dots not at start or end.
	if( (l<7) || (l>15) || (std::count(ip.begin(), ip.end(), '.')!=3) || (ip[0]=='.') || (ip[l-1]=='.') )
		MessageHandler::error(WrongIp, ip);
	unsigned int intIp = 0;
	for(int i=0, j; i<l; i=j+1){
		bool notZero=false, zero=false;
		for(j=i; (ip[j]!='.') && (j<l); j++){
			// char by char comprobations, is a number and there arent left zeros.
			if( (ip[j]<'0') || (ip[j]>'9') || (zero && !notZero) )
				MessageHandler::error(WrongIp, ip);
			if(ip[j]=='0') zero = true;
			else notZero = true;
		}
		int octet = std::stoi(ip.substr(i, j));
		// Final comprobations is an 8 bites number, there arent consecutive points.
		if( !(octet <= 255) || ( (ip[j]=='.') && (ip[j-1]=='.') ) ) 
			MessageHandler::error(WrongIp, ip);
		intIp += octet;
		if(j<l-1) intIp<<=8;
	}
	return intIp;
}

std::string Network::getDefaultInterface(){
	fileManager.open("/proc/net/route");
	std::string line;
	std::string interface;
	std::size_t pos;
	do{
		line = fileManager.getLine();
		pos = line.find("\t");
		interface = line.substr(0, pos);
	// The deafault interface is the one whose destination is 00000000.
	}while(line.substr(pos+1, 8)!="00000000");
	fileManager.close();
	return interface;
}

std::string Network::getLocalIp(int family){
	std::string interface = getDefaultInterface();
	char host[NI_MAXHOST];
	struct ifaddrs *ifaddr;
	if(getifaddrs(&ifaddr) != 0) 
		MessageHandler::error(NoIfaddrs, std::to_string(errno));
	//Searches on all the ifaddrs the one who matches the interface
	for(struct ifaddrs*it=ifaddr; it!=NULL; it=it->ifa_next){
		if(it->ifa_addr==NULL) continue;
		if(strcmp(it->ifa_name, interface.c_str())==0){
			if (it->ifa_addr->sa_family==family){
				size_t size = (family==AF_INET)? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
				if(getnameinfo(it->ifa_addr, size, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
					MessageHandler::error(NoNameinfo, std::to_string(errno));
			}
		}
	}
	freeifaddrs(ifaddr);
	return host;
}

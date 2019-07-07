#include "../lib/Socket.hpp"

Socket::Socket(){
	in_addr_t answerAddres = 0;
	uint16_t answerPort = 0;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("Socket:Constructor"); 
	}
}

Socket::~Socket(){
	Close();
}

int Socket::Bind(int port){
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	int len = sizeof(addr);
	int result = bind(sockfd, (const sockaddr *)&addr, len);
	if(result == -1){
		perror("Socket:Bind");
	}
	return result;
}

int Socket::Read(char * buffer, unsigned int len){
	struct sockaddr_in addr;
	unsigned int addrlen = sizeof(addr);
	int bytes = recvfrom(sockfd, buffer, len, 0, (struct sockaddr *) &addr, &addrlen);
	if(bytes == -1){
		perror("Socket:Read");
	}
	answerPort = ntohs(addr.sin_port);
	answerAddres = addr.sin_addr.s_addr;
	buffer[bytes]='\0';
	return bytes;
}

int Socket::Write(std::string message, std::string ip, uint16_t port){
	port = (port==0)?answerPort:port;								
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr)); 
 	addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
	if(ip != "") 
		inet_aton(ip.c_str(),&addr.sin_addr);
	else 
		addr.sin_addr.s_addr = answerAddres;
	int bytes = sendto(sockfd, message.c_str(), message.length(),0, (const struct sockaddr *) &addr, sizeof(addr)); //MSG_CONFIRM
	if(bytes == -1){
		perror("Socket:Write");
	}
	return bytes;
}

int Socket::Close(){
	int result = close(sockfd);
	if(result == -1){
		perror("Socket:Close");
	}
	return result;
}

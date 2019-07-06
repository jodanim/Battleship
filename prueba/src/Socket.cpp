#include "../lib/Socket.hpp"
#include <iostream>

//	AF_INET(IPv4) AF_INET6(IPv6)
//	SOCK_STREAM(TCP) SOCK_DGRAM(UDP)

Socket::Socket(int port){
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		perror("Socket:Constructor"); 
	this->port = port;
}

Socket::~Socket(){
	Close();
}

int Socket::Bind(){
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//revisar si ocupa la dirección local
	addr.sin_port = htons(port);
	int len = sizeof(addr);
	int result = bind(sockfd, (const sockaddr *)&addr, len);
	if(result == -1)perror("Socket:Bind");
	return result;
}

int Socket::Read(char * buffer, int len){
	int flags = 0;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	socklen_t addrlen;
	int bytes = recvfrom(sockfd, buffer, len, 0, (struct sockaddr *) &addr, &addrlen);//MSG_WAITALL
	if(bytes == -1)perror("Socket:Read");
	buffer[bytes]='\0';
	return bytes;
}

int Socket::Write(const char * buffer, const char * ip){
	struct sockaddr_in addr;
 	addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
	inet_aton(ip,&addr.sin_addr);
	int bytes = sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &addr, sizeof(addr)); //MSG_CONFIRM
	if(bytes == -1)perror("Socket:Write");
	return bytes;
}

int Socket::Close(){
	int result = close(sockfd);
	if(result == -1)perror("Socket:Close");
	return result;
}
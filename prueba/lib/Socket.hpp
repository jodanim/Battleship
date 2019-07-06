#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <string>

class Socket{
	public:
		Socket();
		~Socket();
		int Bind(int port);
		int Read(char * buffer, int len);
		int Write(std::string message, std::string ip = "", uint16_t port = 0);
		int Close();
	private:
		int sockfd;
		uint16_t answerPort;
};
#endif
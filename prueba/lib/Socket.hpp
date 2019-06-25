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
		Socket(int new_sockfd);
		~Socket();
		int Connect(std::string host, int port);
		int Listen(int queue);
		int Bind(int port);
		std::string Read(int len,char*buffer);
		int Write(std::string text, int port);
		Socket* Accept();
		int Shutdown(int);
		int Close();
	private:
		int sockfd;
};

#endif

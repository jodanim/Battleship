#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

class Socket{
	public:
		Socket(char t = 's', bool ipv6 = false);
		Socket(int new_sockfd);
		~Socket();
		int Connect(char * host, int port);
		int Connect(char * host, char * service);
		int Read(char * text, int len);
		int Write(char * text);
		int Shutdown(int);
		int Close();
		int Listen(int queue);
		int Bind(int port);
		Socket* Accept();
		int HostnameToIp(char *hostname, char *ip);
		char * getLocalIp(const char *interface);
	private:
		int sockfd;
};

#endif
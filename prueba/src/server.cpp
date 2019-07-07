#include "../lib/Socket.hpp"
#include <iostream>

#define PORT    1101
#define MAXLINE 1024

int main() {
	int sockfd;
    std::string message = "hello from server";
	char buffer[MAXLINE];
	Socket s;

    s.Bind(PORT);
	int cnt =1;
	while(1){
		int n = s.Read(buffer,MAXLINE);
		printf("Client : %s\n", buffer);
		s.Write("hello");
		printf("Hello message sent.\n");
	}
    return 0;
}
#include "../lib/Socket.hpp"

#define PORT     1101 
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = (char *)"Hello from server"; 
	Socket s;
        
    s.Bind(PORT); 

    int n = s.Read(buffer,MAXLINE,"192.168.0.17",1102);
    printf("Client : %s\n", buffer); 

    s.Write(hello,"192.168.0.17",1102);
    
    printf("Hello message sent.\n");  
      
    return 0; 
} 
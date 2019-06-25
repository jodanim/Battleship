#include "../lib/Socket.hpp"
/**

int main(int argc, char**argv){
	int childpid;
	Socket *s;
	s = new Socket();
	s->Connect("192.168.0.16",1101);
	childpid = fork();
	if(childpid < 0)perror("client: Error de bifurcación.");
	else if (0 == childpid) { 
		s->Write(argv[1],1101);
		_exit(0);	
	}
	s->Close();
    return EXIT_SUCCESS;
}

*/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
// Driver code 
int main() { 
    Socket socket;      
    socket.Write("Hello from client",1101);
    return 0; 
} 

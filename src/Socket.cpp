#include "../lib/Socket.hpp"
Socket::Socket(char t, bool ipv6){
	int type = t=='s'?SOCK_STREAM:SOCK_DGRAM;
	int domain = ipv6? AF_INET6:AF_INET;
	sockfd = socket(domain,type,0);
	if(sockfd==-1)perror("Socket:Constructor"); 
}

Socket::Socket(int new_sockfd){
	sockfd = new_sockfd;
}

Socket::~Socket(){
	Close();
}

int Socket::Connect(char * Host, int Port){
	char ip[100];
	if(Host[0]<'0'||Host[0]>'9'){
		HostnameToIp(Host,ip);	
	}else{
		strcpy(ip,Host);
	}
	struct sockaddr_in  host_addr;
	host_addr.sin_family = AF_INET;
	inet_aton(ip,&host_addr.sin_addr);
        host_addr.sin_port = htons(Port);
        int len = sizeof(host_addr);
	int result = connect(sockfd, (sockaddr *) &host_addr,len);
	if(result == -1)perror("Socket:ConnectIpv4");
	return result;
}

int Socket::Connect( char *host, char *service ) {
    size_t len;
    int st;
    struct addrinfo hints, *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          
    
    st = getaddrinfo( host, service, &hints, &result);
	if(st != 0){
		printf("%s\n",gai_strerror(st));
	}

    for ( rp = result; rp; rp = rp->ai_next ) {
        st = connect( sockfd, rp->ai_addr, rp->ai_addrlen );
		if(st == -1)perror("Socket:ConnectIpv6");
        if(st == 0)break;
    }

    freeaddrinfo( result );

    return st;
}

int Socket::Read(char * text, int len){
	memset(text, 0, len);
	int result = read(sockfd,text,len);
	if(result == -1)perror("Socket:Read");
	return result;
}

int Socket::Write(char * text){
	int len = strlen(text);
	int result = write(sockfd,text,len);
	if(result == -1)perror("Socket:Write");
	return result;
	
}

int Socket::Shutdown(int o){
	int result = shutdown(sockfd,o);
	if(result == -1)perror("Socket:Shutdown");
	return result;
}

int Socket::Close(){
	int result = close(sockfd);
	if(result == -1)perror("Socket:Close");
	return result;
}

int Socket::Listen(int Queue){
	int result = listen(sockfd,Queue);
	if(result == -1)perror("Socket:Listen");
	return result;
}

int Socket::Bind(int Port){
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(Port);
	int len = sizeof(server_addr);
	int result = bind(sockfd,(const sockaddr *)&server_addr,len);
	if(result == -1)perror("Socket:Bind");
	return result;
}

Socket * Socket::Accept(){
	struct sockaddr_in cli_addr;
	int clilen = sizeof(cli_addr);
	int new_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
	if(new_sockfd==-1)perror("Socket:Accept");
	return new Socket(new_sockfd);
}

int Socket::HostnameToIp(char *hostname , char *ip){
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	if ( (he = gethostbyname(hostname) ) == NULL){
		perror("Socket:gethostbyname");
		return 1;
	}
	addr_list = (struct in_addr **) he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++){
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}
	return 1;
}
#include "../lib/IPC/Mailbox.hpp"
#include <iostream>

Mailbox::Mailbox(){
	if((msqid = msgget(KEY, 0600|IPC_CREAT)) == -1){
		perror("Mailbox:cosntructor");
		exit(1);
	}
}

Mailbox::~Mailbox(){
	if(msgctl(msqid, IPC_RMID, NULL) == -1){
		perror("Mailbox:destructor");
		exit(1);
	}
}

void Mailbox::send(std::string message, int type){
	struct msgbuf msgp;
	msgp.mtype = type;
	size_t msgsz = message.size()>SIZE?;
	msgp.message = message;
	
	int msgflg = IPC_NOWAIT;
	if(msgsnd(msqid, (void *) &msgp, msgsz, msgflg) == -1){
		perror("Mailbox:send");
		exit(1);
	}
}


std::string Mailbox::receive(int type){
	struct msgbuf msgp;
	size_t msgsz = sizeof(msgp)-sizeof(long);
	int msgflg = IPC_NOWAIT|MSG_NOERROR;
	if(-1==msgrcv(msqid, (void*) &msgp, msgsz, type, msgflg)){
		if (errno != ENOMSG){
		   perror("Mailbox:receive");
		   exit(1);
       }
       printf("No message available.\n");
		return NULL;
	}
	std::cout<<"Mensaje: "<<msgp.message;
	return;
}

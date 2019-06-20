#include "../../lib/IPC/Mailbox.hpp"

Mailbox::Mailbox(){
	if((msqid = msgget(key, 0600|IPC_CREAT)) == -1)
		MessageHandler::error(msggetErr,std::to_string(errno));
}

Mailbox::~Mailbox(){
	if(msgctl(msqid, IPC_RMID, NULL) == -1)
		MessageHandler::error(msgctlErr,std::to_string(errno));
}

void Mailbox::send(std::string message, int type){
	struct msgbuf msgp;
	msgp.mtype = type;
	size_t msgsz = message.size()>MAILBOX_MAX_MESSAGE_SIZE?MAILBOX_MAX_MESSAGE_SIZE:message.size();
	strncpy(msgp.message,message.c_str(),msgsz);
	int msgflg = IPC_NOWAIT;
	if(msgsnd(msqid, (void *) &msgp, msgsz, msgflg) == -1)
		MessageHandler::error(msgsndErr,std::to_string(errno));
}

std::string Mailbox::receive(int type){
	struct msgbuf msgp;
	size_t msgsz = sizeof(msgp)-sizeof(long);
	int msgflg = IPC_NOWAIT|MSG_NOERROR;
	if(msgrcv(msqid, (void*) &msgp, msgsz, type, msgflg) == -1){
		if(errno != ENOMSG)
			MessageHandler::error(msgrcvErr,std::to_string(errno));
		else
			MessageHandler::msg(noIPCMessages);
		return "";
	}
	return std::string(msgp.message);
}
#ifndef MAILBOX_HPP
#define MAILBOX_HPP

#include "../MessageHandler.hpp"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <string>

#define MAILBOX_MAX_MESSAGE_SIZE 64

class Mailbox{
public:
	Mailbox();
	~Mailbox();
	void send(std::string message, int type);
	std::string receive(int type);
private:
	key_t key = 0xB44007;
	int msqid;
	struct msgbuf{
		long mtype;
		char message[MAILBOX_MAX_MESSAGE_SIZE];
	};
};

#endif /* MAILBOX_HPP */

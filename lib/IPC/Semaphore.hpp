#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define KEY 0xB44007

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

union semun{
	int val;
};

class Semaphore{
    public:
        Semaphore(int initialValue = 0);
        ~Semaphore();
        int signal(int n = 1);
        int wait(int n = 1);
    private:
        int id;
};

#endif /* SEMAPHORE_H */


#include "../../lib/IPC/Semaphore.hpp"

Semaphore::Semaphore(int vi){
	id = semget(KEY,1,0600|IPC_CREAT);
	if(-1==id){
		perror("semaphore:constructor");
		exit(1);
	}

	semun a;
	a.val = vi;
	if(-1==semctl(id,0,SETVAL,a)){
		perror("semaphore:constructor");
		exit(1);
	}
}

Semaphore::~Semaphore(){
	if(-1==semctl(id,0,IPC_RMID)){
		perror("semaphore:destructor");
		exit(1);
	}
}

int Semaphore::signal(int n){
	sembuf s;
	s.sem_num = 0;
	s.sem_op = n;
	s.sem_flg = 0;
	if(-1==semop(id,&s,1)){
		perror("semaphore:signal");
		return 0;
	}
	return 1;
}

int Semaphore::wait(int n){
	sembuf s;
	s.sem_num = 0;
	s.sem_op = -n;
	s.sem_flg = 0;
	if(-1==semop(id,&s,1)){
		perror("semaphore:wait");
		return 0;
	}
	return 1;
}


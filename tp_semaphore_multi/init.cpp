#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <sys/msg.h> 
#include <iostream>

using namespace std;

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
};

int main(int argc, char** const argv){

	if(argc != 2){
		cout<<"nb d'arg incorrect / use : ./setup nbZone"<<endl;
		exit(EXIT_FAILURE);
	}

	cout<<"Création clef semaphore"<<endl;

	errno=0;
	key_t clefSem = ftok("./toto.txt",10);
	if(clefSem==-1){
		perror("Erreur ftok()");
		exit(EXIT_FAILURE);
	}

	cout<<"initialisation semaphore"<<endl;

	errno=0;
	int idSem = semget(clefSem, atoi(argv[1]),IPC_CREAT|0666);
	if(idSem < 0){
		perror("Erreur semget()");
		exit(EXIT_FAILURE);
	}

}
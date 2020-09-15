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


int main(int argc, char** const argv){
cout<<"Création clef semaphore"<<endl;

	errno=0;
	key_t clefSem = ftok("./toto.txt",10);
	if(clefSem==-1){
		perror("Erreur ftok()");
		exit(EXIT_FAILURE);
	}

	cout<<"initialisation semaphore"<<endl;

	errno=0;
	int idSem = semget(clefSem, 1,IPC_CREAT|0666);
	if(idSem < 0){
		perror("Erreur semget()");
		exit(EXIT_FAILURE);
	}

	
	errno=0;
	int res = semctl(idSem,0, IPC_RMID);
	if(res < 0){
		perror("Erreur semctl()");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;

}
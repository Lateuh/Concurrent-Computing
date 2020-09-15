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

struct sembuf op[]{
	{(u_short)0,(short)-1,0}, // p
	{(u_short)0,(short)0,0}, // z
	{(u_short)0,(short)1,0} // v
};


int main(int argc, char** argv){

	if(argc != 1){
		cout<<"nb d'arg incorrect"<<endl;
		exit(EXIT_FAILURE);
	}

	errno=0;
	key_t clefSem = ftok("./toto.txt",10);
	if(clefSem==-1){
		perror("Erreur ftok()");
		exit(EXIT_FAILURE);
	}


	errno=0;
	int idSem = semget(clefSem,1,0666);
	if(idSem < 0){
		perror("Erreur semget()");
		exit(EXIT_FAILURE);
	}

	errno=0;
	int res = semop(idSem,&op[0],1);
	if(res < 0){
		perror("Erreur semop()");
		exit(EXIT_FAILURE);
	}

	cout<<"J'attends que toutes les ressources soient utilisées"<<endl;

	sleep(10);

	errno=0;
	res = semop(idSem,&op[2],1);
	if(res < 0){
		perror("Erreur semop()");
		exit(EXIT_FAILURE);
	}

	cout<<"Je suis debloque, je passe à la suite"<<endl;



	return EXIT_SUCCESS;
}
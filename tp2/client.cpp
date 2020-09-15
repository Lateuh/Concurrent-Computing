#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <iostream>

using namespace std;
/* Partie commune aux clients et au serveur */

typedef struct { 
    long etiquette; //1
    int nb1;
    char op[1];
    int nb2; 
} CALCUL; 


typedef struct{
	long etiquette; //2
	int resultat;
} RESULTAT;

/* Fin de la partie commune aux clients et au serveur */

int main(int argc, char** argv){
	if(argc!=1){
		printf("Erreur argument prog. Utilisation : ./client.cpp");
		exit(EXIT_FAILURE);
	}

	CALCUL c1;

	c1.etiquette = 1;
	cin>>c1.nb1;

	cin>>c1.op[0];

	cin>>c1.nb2;



	errno=0;
	key_t laclef=ftok("./toto.txt",10);
	if(laclef==-1){
		perror("Erreur ftok()");
		exit(EXIT_FAILURE);
	}

	errno=0;
	int id_f = msgget(laclef,IPC_CREAT|0666);
	if(id_f==-1){
		perror("Erreur msgget()");
		exit(EXIT_FAILURE);
	}

	errno=0;
	int res = msgsnd(id_f,&c1,sizeof(c1)-sizeof(long),0);
	if(res==-1){
		perror("Erreur msgsnd()");
		exit(EXIT_FAILURE);
	}


	RESULTAT r1;
	r1.etiquette=2;


	cout<<"on attend le message"<<endl;


	errno=0;
	res = msgrcv(id_f,&r1,sizeof(c1)-sizeof(long),2,0);
	if(res==-1){
		perror("Erreur msgsnd()");
		exit(EXIT_FAILURE);
	}

	cout<<"résultat "<<r1.resultat<<endl;

	return EXIT_SUCCESS;
}
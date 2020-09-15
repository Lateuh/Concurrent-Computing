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
    pid_t myPid;
} CALCUL; 


typedef struct{
	long etiquette; //2
	int resultat;
} RESULTAT;

/* Fin de la partie commune aux clients et au serveur */

int main(int argc, char** argv){

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

	while(1){

		CALCUL c1;

		cout<<"on attend le message"<<endl;
		errno=0;
		int res = msgrcv(id_f,&c1,sizeof(c1)-sizeof(long),1,0);
		if(res==-1){
			perror("Erreur msgrcv()");
			exit(EXIT_FAILURE);
		}


		RESULTAT r1;
		r1.etiquette=c1.myPid;

		switch (c1.op[0]){
			case '+' : r1.resultat=c1.nb1+c1.nb2;
			break;

			case '-' : r1.resultat=c1.nb1-c1.nb2;
			break;

			case '*' : r1.resultat=c1.nb1*c1.nb2;
			break;

			case '/' : r1.resultat=c1.nb1/c1.nb2;
			break;

			default : cout<<"pas compatible"<<endl;

		}

		cout<<"résultat "<<r1.resultat<<endl;

		
		errno=0;
		res = msgsnd(id_f,&r1,sizeof(r1)-sizeof(long),0);
		if(res==-1){
			perror("Erreur msgsnd()");
			exit(EXIT_FAILURE);
		}

	}

	return EXIT_SUCCESS;
}
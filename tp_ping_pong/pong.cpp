#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<iostream>


using namespace std;


int main(int argc, char *argv[])
{

	if(argc != 2){
		printf("Utilisation : ./recep 'numeroPort'\n");
		exit(EXIT_FAILURE);
	}


	struct sockaddr_in addrS;

	addrS.sin_family = AF_INET;
	addrS.sin_addr.s_addr = INADDR_ANY;
	addrS.sin_port = htons(atoi(argv[1]));


	errno=0;
	int dS = socket(PF_INET,SOCK_DGRAM,0);
	if(dS==-1){
		perror("Erreur socket() : ");
		exit(EXIT_FAILURE);
	}


	errno=0;
	int res = bind(dS,(struct sockaddr *) &addrS,sizeof(struct sockaddr_in));
	if(res==-1){
		perror("Erreur bind() : ");
		exit(EXIT_FAILURE);
	}



	struct sockaddr_in addrC;
	socklen_t lgA = sizeof(struct sockaddr_in);

	
	
	//char *msg=malloc(sizeof(char)*50);


	char msg[50];

	errno=0;
	res=recvfrom(dS, msg,50,0,(struct sockaddr *)&addrC,&lgA);
	if(res==-1){
		perror("Erreur recvfrom() : ");
		exit(EXIT_FAILURE);
	}


	printf("On a reçu %d octets : %s \n",res, msg);
	int taille=strlen(msg);
	printf("taille msg : %d\n",taille);
	
	errno=0;
	res=sendto(dS,&taille,sizeof(int),0,(struct sockaddr *)&addrC,lgA);
	if(res==-1){
		perror("Erreur sendto() : ");
		exit(EXIT_FAILURE);
	}

	printf("On a renvoyé %d octets.\n",res);





	char msg2[20];

	errno=0;
	res=recvfrom(dS, msg2, 50,0,(struct sockaddr *)&addrC,&lgA);
	if(res==-1){
		perror("Erreur recvfrom() : ");
		exit(EXIT_FAILURE);
	}


	printf("On a reçu %d octets : %s \n",res, msg2);
	taille=strlen(msg2);
	printf("taille msg : %d\n",taille);

	struct message{
		int n1;
		int n2;
		char mot[10];
	};

	message m1;
	m1.n1 = 2;
	m1.n2 = 3;
	// m1.mot = "salut";
	strncpy(m1.mot,"salut",sizeof(m1.mot));

	errno=0;
	res=sendto(dS,&m1,sizeof(message),0,(struct sockaddr *)&addrC,lgA);
	if(res==-1){
		perror("Erreur sendto() : ");
		exit(EXIT_FAILURE);
	}

	printf("On a renvoyé %d octets.\n",res);


	close(dS);

	return 0;
}
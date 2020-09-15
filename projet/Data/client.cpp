#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iostream>


using namespace std;


int recvTCP(int desc, char* pointeur, int
	size, int flag){
	int recu = 0;
	while(recu<size){
		int nbRecu = recv(desc, pointeur+recu, size-recu, 0);
		if (nbRecu==-1){
			return -1;
		}
		else if (nbRecu==0){
			return 0;
		}
		else{
			recu+=nbRecu;
		}
	}
	return recu;
}

int sendTCP(int desc, char* pointeur, int size, int flag){
	int envoye = 0;
	while(envoye<size){
		int nbEnvoye = send(desc, pointeur+envoye, min(size-envoye,2048), 0);
		if (nbEnvoye==-1){
			return -1;
		}
		else if (nbEnvoye==0){
			return 0;
		}
		else{
			envoye+=nbEnvoye;
		}
	}
	return envoye;
}



void sendFile(int dS, char* name){
	struct stat statFile;
	FILE* file;
	int size;

	if ((file = fopen(name,"r")) == NULL)
	{
		perror("Impossible d'ouvrir le fichier données en lecture\n");
		size=-1;
	}
	if(size!=-1){
		stat(name, &statFile); 
		char contenu[2048];
		int aEnvoyer=strlen(name)+1;
		int nameFile=sendTCP(dS, (char*)&aEnvoyer, sizeof(int), 0);
		if (nameFile==-1)//Teste le sendTCP
		{
			perror("Erreur sendTCP nameFile");
			exit(0);
		}
		nameFile=sendTCP(dS, name, strlen(name)+1, 0);
		if (nameFile==-1)//Teste le sendTCP
		{
			perror("Erreur sendTCP nameFile");
			exit(0);
		}
		int sizeFile=sendTCP(dS, (char*)&statFile.st_size, sizeof(off_t), 0);
		if (sizeFile==-1)//Teste le sendTCP
		{
			perror("Erreur sendTCP sizeFile");
			exit(0);
		}
		int lu;
		while((lu = fread(contenu, 1, 2048, file))>0){
			cout<<"nbLu : "<<lu<<endl;
			int contenuFile = sendTCP(dS, contenu, lu, 0);
			cout<<"nbEnvoye : "<<contenuFile<<endl;
			if (contenuFile==-1)
			{
				perror("Erreur sendTCP contenuFile");
				exit(0);
			}
		}
		fclose(file);
	}
	else{
		int err=sendTCP(dS, (char*)&size, sizeof(int), 0);
		if (err==-1)//Teste le sendTCP
		{
			perror("Erreur sendTCP nameFile");
			exit(0);
		}
	}
}


void download(int dS, char* name){

	int aEnvoyer=strlen(name)+1;
	int nameFile=sendTCP(dS, (char*)&aEnvoyer, sizeof(int), 0);
	if (nameFile==-1)//Teste le sendTCP
	{
		perror("Erreur sendTCP nameFile");
		exit(0);
	}
	int envoiName = sendTCP(dS, name, strlen(name)+1, 0);
	if (envoiName==-1)//Teste le sendTCP
	{
		perror("Erreur sendTCP name");
		exit(0);
	}

	off_t sizeFile;
	int recutaille=recvTCP(dS, (char*)&sizeFile, sizeof(off_t),0);
	if (recutaille==-1)//Teste le recvTCP
	{
		perror("Erreur recvTCP recutaille");
		exit(0);
	}
	if (sizeFile!=-1){

		system("mkdir Downloads");

		char cUp[110] = "./Downloads/";
		strcat(cUp,name);

		FILE* file;
		if((file = fopen(cUp,"w")) == NULL){
			perror("Impossible d'ouvrir le fichier données en lecture\n");
			exit(0);
		}

		char buffer[2048];
		int ecrit = 0;
		while(ecrit<sizeFile){
			int recu=recvTCP(dS, buffer, min((int)sizeFile-ecrit,2048), 0);
			if (recu<=0)
			{
				perror("Erreur recv3");
			}
			ecrit += fwrite(buffer, 1, recu, file);
			if (ecrit > 1000)
			{
				cout<<"nbEcrit total : "<<ecrit/1000<<"Ko"<<endl;
			}
			else if (ecrit > 1000000)
			{
				cout<<"nbEcrit total : "<<ecrit/1000000<<"Mo"<<endl;
			}
			else if (ecrit > 1000000000)
			{
				cout<<"nbEcrit total : "<<ecrit/1000000000<<"Go"<<endl;	
			}
			else
			{
				cout<<"nbEcrit total : "<<ecrit<<"o"<<endl;
			}


		}
		fclose(file);

	}


}



void checkReponse(int dS, char reponse[2048]){
	if (strcmp(reponse,"Session terminée")!=0)
	{
		if (strcmp(reponse,"Upload : Quel fichier ?")==0)
		{
			cout<<reponse<<endl;
			char name[100];
			cin>>name;
			sendFile(dS, name);
		}

		else if (strcmp(reponse,"Download : Quel fichier ?")==0)
		{
			cout<<reponse<<endl;
			char name[100];
			cin>>name;
			download(dS, name);
		}

		else if (strcmp(reponse,"Delete : Quel fichier ?")==0||strcmp(reponse,"Read : Quel fichier ?")==0)
		{
			cout<<reponse<<endl;
			char name[100];
			cin>>name;
			int aEnvoyer=strlen(name)+1;
			int nameFile=sendTCP(dS, (char*)&aEnvoyer, sizeof(int), 0);
			if (nameFile==-1)//Teste le sendTCP
			{
				perror("Erreur sendTCP nameFile");
				exit(0);
			}
			int envoiDelRead=sendTCP(dS, name, strlen(name)+1, 0);
			if (envoiDelRead==-1)//Teste le sendTCP
			{
				perror("Erreur sendTCP envoi");
				exit(0);
			}
			cout<<"En attente..."<<endl;
		}

		else if (strcmp(reponse,"Write : Quel fichier ?")==0)
		{
			cout<<reponse<<endl;
			char name[100];
			cin>>name;
			int aEnvoyer=strlen(name)+1;
			int nameFile=sendTCP(dS, (char*)&aEnvoyer, sizeof(int), 0);
			if (nameFile==-1)//Teste le sendTCP
			{
				perror("Erreur sendTCP nameFile");
				exit(0);
			}
			int envoiDelRead=sendTCP(dS, name, strlen(name)+1, 0);
			if (envoiDelRead==-1)//Teste le sendTCP
			{
				perror("Erreur sendTCP envoi");
				exit(0);
			}

			cout<<"En attente..."<<endl;



			char modif[100];
			cout<<"Maximum un mot de 100 caractères : "<<endl;
			cin>>modif;



			int envoiModif=sendTCP(dS, modif, strlen(modif)+1, 0);
			if (envoiModif==-1)//Teste le sendTCP
			{
				perror("Erreur sendTCP modif");
				exit(0);
			}

		}
		else if (strcmp(reponse,"View :")==0 || strcmp(reponse,"Historique :")==0)
		{
			int size;
			int recu=recvTCP(dS, (char*)&size, sizeof(off_t),0);
			if (recu==-1)//Teste le recvTCP
			{
				perror("Erreur recvTCP recu");
				exit(0);
			}

			char* buffer=(char*)calloc(2048,sizeof(char));
			int ecrit = 0;
			while(ecrit<size){
				int recu=recvTCP(dS, buffer, min((int)size-ecrit,2048), 0);
				if (recu<=0)
				{
					perror("Erreur recv3");
				}
				ecrit += recu;
				cout<<buffer;
			}
			cout<<endl;
		}
		else
		{
			cout<<reponse<<endl;
			char requete[2048];
			cin>>requete;
			int aEnvoyer=strlen(requete)+1;
			int nameFile=sendTCP(dS, (char*)&aEnvoyer, sizeof(int), 0);
			if (nameFile==-1)//Teste le sendTCP
			{
				perror("Erreur sendTCP nameFile");
				exit(0);
			}
			int envoi=sendTCP(dS, requete, strlen(requete)+1, 0);
			if (envoi==-1)//Teste le sendTCP
			{
				perror("Erreur sendTCP envoi");
				exit(0);
			}
		}

	}
}


int main(int argc, char const *argv[])
{

//argv[1]: adresse
//argv[2]: port


	if (argc != 3){
		printf("Syntaxe : %s [IP du serveur] [Port du serveur] \n", argv[0]);
		exit(0);
	}


	struct sockaddr_in ad ;
	ad.sin_family=AF_INET;
	inet_pton(AF_INET, argv[1], &(ad.sin_addr));
	ad.sin_port=htons(atoi(argv[2]));
	socklen_t lgA=sizeof(struct sockaddr_in);

	int dS=socket(PF_INET, SOCK_STREAM, 0);
	if (dS==-1)//Verifie que la socket ai bien été crée
	{
		fprintf(stderr,"Erreur socket\n");
		exit(1);
	}
	
	if(connect(dS, (struct sockaddr*) &ad, lgA)!=0) //Teste la connexion
	{
		fprintf(stderr,"Impossible de se connecter\n");
		exit(1);		
	}



	// char* reponse = (char*) malloc(2048 * sizeof(char));
	char reponse[2048];
	while(strcmp(reponse,"Session terminée")!=0)
	{
		// free(reponse);
		int aRecevoir;
		int recu=recvTCP(dS, (char*)&aRecevoir, sizeof(int),0);
		if (recu==-1)//Teste le recvTCP
		{
			perror("Erreur recvTCP recu");
			exit(0);
		}
		recu=recvTCP(dS, reponse, aRecevoir,0);
		if (recu==-1)//Teste le recvTCP
		{
			perror("Erreur recvTCP recu");
			exit(0);
		}
		checkReponse(dS, reponse);
	}
	cout<<"Deconnexion"<<endl;
	close(dS);

	return 0;
}
#include<pthread.h>
#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"calcul.h"

using namespace std;

struct paramT{
	int cpt;
	pthread_mutex_t verrou;
	pthread_cond_t condi;
};


void* monThread (void* par){

	paramT* pT =(paramT*) par;
	int id=pT->cpt;

	cout<<"debut thread "<<id<< endl;

	pT->cpt--;

	pthread_mutex_lock(&(pT->verrou));

	calcul(2);
	while(id<=5 && id>0){
		pthread_cond_wait(&(pT->condi),&(pT->verrou));

	}

	cout<<"fin thread "<<id<<endl;

	pthread_mutex_unlock(&(pT->verrou));



	
	pthread_exit(NULL);
}





int main(int argc, char** argv){

	if(argc!=2){
		cout<<"Nombre d'arguments incorrect. 2 arguments attendus : ./exo3 nb_thread"<<endl;
		exit(EXIT_FAILURE);
	}

	cout<<"debut main"<<endl;
	paramT para;

	para.cpt=atoi(argv[1]);
	pthread_t idTh;

	for(int i = para.cpt;i>0;i--){
		if (pthread_create(&idTh, NULL, monThread,&para) != 0){
			cout << "erreur creation thread" <<endl;	
			exit(EXIT_FAILURE);
		}
	}

	// if (pthread_create(&idTh2, NULL, monThread,&para) != 0){
	// 	cout << "erreur creation" <<endl;	
	// 	exit(EXIT_FAILURE);
	// }

	// int res = pthread_join(idTh, &retval);
	// if (res!=0){
	// 	cout<<"erreur attente thread"<<endl;
	// 	exit(EXIT_FAILURE);
	// }

	int res = pthread_join(idTh, NULL);
	if (res!=0){
		cout<<"erreur attente thread"<<endl;
		exit(EXIT_FAILURE);
	}


	cout<<"fin main"<<endl;

	return 0;
}
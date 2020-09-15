#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <iomanip>
#include <vector>

using namespace std;



int x;
pthread_mutex_t verrou;
pthread_cond_t changeValX;

void * f1 (void * p){

	cout<<" f1 commence"<<endl;

	pthread_mutex_trylock(&verrou);
	cout<<"f1_attend que x soit > 10"<<endl;
	while(x<=10)
	{
		pthread_cond_wait(&changeValX, &verrou);
	}
	pthread_mutex_unlock(&verrou);

	cout<<"f1 commence son calcul"<<endl;

	cout<<"f1 termine"<<endl;

	pthread_exit(NULL);

}


void * f2(void * p){

	cout<<" f2 commence"<<endl;

	pthread_mutex_lock(&verrou);
	cout<<"f2 modifie x"<<endl;
	x=20;
	pthread_mutex_unlock(&verrou);

	cout<<"f2 reveille f1"<<endl;
	pthread_cond_signal(&changeValX);
	cout<<"f2 se termine"<<endl;

}


int main(){
	pthread_t idf1, idf2;
	x=5;
	pthread_mutex_init(&verrou,NULL);
	pthread_cond_init(&changeValX,NULL);
	pthread_create(&idf1,NULL,f1,NULL);
	pthread_create(&idf2,NULL,f2,NULL);
	pthread_join(idf1,NULL);
	pthread_join(idf2,NULL);
}
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>


void* monThread(void* par) {

pthread_t moi = pthread_self();
printf("thread %d, proc. %d",moi,getpid());
pthread_exit(NULL);
}


int main(){

pthread_t idTh;
if(pthread_create(&idTh,NULL,monThread,NULL) != 0){
	printf("erreur creation thread");
}

	wait(3);
	
	return 0;
}
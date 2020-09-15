#include<pthread.h>
#include<iostream>
#include<sys/types.h>
#include<unistd.h>

using namespace std;

void* monThread (void*){
	cout<<"debut fonc thread"<< endl;
	pthread_t moi = pthread_self();
	cout<< "je suis le thread " << moi << ", proc. de pid "<< getpid()<< endl;
	cout<<"fin fonc thread"<< endl;
	
	pthread_exit(NULL);
}


int main(){

	cout<<"debut main"<<endl;
	cout<<"pid main : "<<getpid()<<endl;
	pthread_t idTh, idTh2;
	if (pthread_create(&idTh, NULL, monThread, NULL) != 0){
		cout << "erreur creation" <<endl;	
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&idTh2, NULL, monThread, NULL) != 0){
		cout << "erreur creation" <<endl;	
		exit(EXIT_FAILURE);
	}

	int res = pthread_join(idTh, NULL);
	if (res!=0){
		cout<<"erreur attente thread"<<endl;
		exit(EXIT_FAILURE);
	}
	cout<<"on attend la fin du thread"<<endl;

	cout<<"fin main"<<endl;

	return 0;
}
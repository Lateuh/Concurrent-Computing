#include<pthread.h>
#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

using namespace std;


void* multipFred (void* par){

	int* nimp=(int*) par;
	nimp[i]*nimp[i+1];
	par=(void*) nimp;
	pthread_exit(&par);

}


void* addFred(void* par){

}

int main(int argc, char const *argv[])
{


	if(argc!=2){
		printf("Erreur, dimension des vecteurs attendue en argument. \n");
		exit(EXIT_FAILURE);
	}

	int dimension = argv[1];

	

	/*int vectA[2*argv[1]];
	int vectB[argv[1]];
	
	
	for(int i=0;i<(2*argv[1]);i++){
		srand(time(NULL));
		vectA[i]=rand() % 6;
		//vectB[i]=rand() % 6;
	}

	int tabX[2];

	for(int i=0;i<argv[1];i++){
		tabVect[0][i]=vectA[i];
		tabVect[1][i]=vectB[i];
	}
*/
	for(int i=0;i<argv[1];i++){
		pthread_t idTh;
		if (pthread_create(&idTh, NULL, multi, &vectA) != 0){
		cout << "erreur creation" <<endl;	
		exit(EXIT_FAILURE);
		}
	}

	pthread_t idTh2;
	

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


	return EXIT_SUCCESS;
}
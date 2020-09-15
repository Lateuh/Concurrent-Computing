
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h>
#include "calcul.h"
using namespace std;




int main(int argc, char const *argv[])
{
        if (argc != 3)
        {
                cout<<"Mauvaise syntaxe : \"./traitement  nbzones N°_traitement\"\n"<<endl;
                return -1;
        }

        int numTraitement = atoi(argv[2]);

        if (numTraitement > 2)
        {
                cout<<"Numero incorrect : 2 traitements maximum\n"<<endl;
                return -1;
        }


///////////////////////////////////////////////////////
        key_t cle = ftok("toto.txt",7);
        if (cle == -1)
        {
                cout<<"erreur du ftok()"<<endl;
                exit(2);
        }

        int semid = semget(cle, 1, 0666);
        if (semid < 0)
        {
                perror("erreur semget()");
                exit(EXIT_FAILURE);
        }

///////////////////////////////////////////////////////

        int nbZone = atoi(argv[1]);


if (numTraitement == 1)
{

        for (int i = 1; i <= nbZone; ++i)
        {
                cout<<"Traitement 1 de la zone: "<<i<<"."<<endl;
                sleep(5);
                struct sembuf V1 = {0,(short)i,0};
                int V = semop(semid, &V1, 1); // V i sur S[0]
                if (V < 0)
                {
                        perror("erreur semop() V");
                        exit(EXIT_FAILURE);
                }

        }
        cout<<"Traitement 1 fini sur toutes les zones."<<endl;
}

if (numTraitement == 2)
{
        for (int i = 1; i <= nbZone; ++i)
        {
                cout<<"J'attend que le traitement 1 soit fini sur la zone: "<<i<<endl;
                struct sembuf Pi ={0,(short)-i, 0};
                int P = semop(semid, &Pi, 1); //P i sur S[0]
                if (P < 0)
                {
                        perror("erreur semop() P");
                        exit(EXIT_FAILURE);
                }

                cout<<"Traitement 2 de la zone: "<<i<<"."<<endl;
                sleep(5);
        }
                cout<<"Traitement 2 fini sur toutes les zones."<<endl;
}

//cout<<"Toutes les zones sont traitées"<<endl;


        return 0;
}
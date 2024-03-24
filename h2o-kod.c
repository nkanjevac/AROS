#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define MOLEKULI 10

int kiseonik = 0;
int vodonik = 0;
int spremniZaVezu = 0;

sem_t mutex;
sem_t semKiseonik;
sem_t semVodonik;


int funkcijaVeze() {

    spremniZaVezu++;

    if(spremniZaVezu % 3 == 0) {
        printf("\nKreiran je novi molekul H2O\n");
        spremniZaVezu = 0;
    }

    sleep(1);
    return 0;

}

void* funkcijaKiseonika() {

        sleep(1);

        sem_wait(&mutex);
        kiseonik+=1;

        if(vodonik>=2) {

            sem_post(&semKiseonik);
            kiseonik-=1;

            sem_post(&semVodonik);
            sem_post(&semVodonik);
            vodonik-=2;

        }
        else {

            sem_post(&mutex);

        }
        
        sem_wait(&semKiseonik);
        printf("Molekul O je spreman za H20\n");

        funkcijaVeze();
        sem_post(&mutex);

}

void* funkcijaVodonika() {

        sleep(1);

        sem_wait(&mutex);
        vodonik+=1;

        if(vodonik>=2 && kiseonik>=1) {

            sem_post(&semVodonik);
            sem_post(&semVodonik);
            vodonik-=2;

            sem_post(&semKiseonik);
            kiseonik-=1;

        }
        else {

            sem_post(&mutex);

        }

        sem_wait(&semVodonik);
        printf("Molekul H je spreman za H20\n");
        funkcijaVeze();

}

int main() {

    printf("Program počinje sa radom\n");
    
    sem_init(&mutex,0,1);
    sem_init(&semKiseonik,0,0);
    sem_init(&semVodonik,0,0);

    printf("Semafori su inicijalizovni\n");        
    sleep(1);

    pthread_t nitiKiseonika[MOLEKULI];
    pthread_t nitiVodonika[2*MOLEKULI];

    for (int i=0; i<MOLEKULI; i++) {
        pthread_create(&nitiKiseonika[i], NULL, funkcijaKiseonika, NULL);
    }
    
    for (int i=0; i<2*MOLEKULI; i++) {
        pthread_create(&nitiVodonika[i], NULL, funkcijaVodonika, NULL);
    }

    printf("\n\n\nNiti su kreirane\n\n\n");        
    sleep(1);

    for(int i = 0; i < MOLEKULI; i++) {
        pthread_join(nitiKiseonika[i], NULL);
    }

    for(int i = 0; i < 2*MOLEKULI; i++) {
        pthread_join(nitiVodonika[i], NULL);
    }

    printf("\n\nProgram je završio sa radom");

}

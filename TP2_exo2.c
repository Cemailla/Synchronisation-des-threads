#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 

#define N_THREADS 4
#define DELAY 100000 // Délai de 1/10 de seconde en microsecondes

void* print_message(void* arg) {
    int thread_number = *((int*)arg); // Récupération du numéro du thread
    for (int i = 0; i < 10; i++) { // Afficher 10 fois pour l'exemple
        printf("Je suis le thread N°%d\n", thread_number);
        usleep(DELAY); // Pause de 1/10 de seconde
    }
    return NULL;
}

int main() {
    pthread_t threads[N_THREADS];
    int thread_numbers[N_THREADS];

    // Création des threads
    for (int i = 0; i < N_THREADS; i++) {
        thread_numbers[i] = i + 1;
        pthread_create(&threads[i], NULL, print_message, &thread_numbers[i]);
    }

    // Attente de la fin des threads
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

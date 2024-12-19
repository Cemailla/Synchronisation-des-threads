#include <stdio.h>
#include <pthread.h>

#define N_THREADS 5
#define N_INCREMENTS 10000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Initialisation du mutex
int cpt = 0; // Variable locale du thread principal

void* increment(void* arg) {
    for (int i = 0; i < N_INCREMENTS; i++) {
        pthread_mutex_lock(&mutex); // Verrouillage
        cpt += 1;
        pthread_mutex_unlock(&mutex); // Déverrouillage
    }
    return NULL;
}

int main() {
    pthread_t threads[N_THREADS];

    // Création des threads
    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    // Attente de la fin des threads
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Affichage de la valeur finale du compteur
    printf("Valeur finale du compteur : %d\n", cpt);

    // Destruction du mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

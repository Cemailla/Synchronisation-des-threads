#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NB_PHILOSOPHES 3
#define NB_REPAS 5

pthread_mutex_t baguettes[NB_PHILOSOPHES];
pthread_mutex_t affichage_mutex;

typedef struct {
    int id;
    int repas;
} Philosophe;

void *philosopher(void *arg) {
    Philosophe *philo = (Philosophe *)arg;

    while (philo->repas < NB_REPAS) {
        pthread_mutex_lock(&affichage_mutex);
        for (int i = 0; i < NB_PHILOSOPHES; i++) {
            if (i == philo->id) {
                printf("Philosophe %d mange (repas %d)...\n", philo->id + 1, philo->repas + 1);
            } else {
                printf("Philosophe %d pense...\n", i + 1);
            }
        }
        pthread_mutex_unlock(&affichage_mutex);
        sleep(1);

        pthread_mutex_lock(&baguettes[philo->id]);
        pthread_mutex_lock(&baguettes[(philo->id + 1) % NB_PHILOSOPHES]);

        pthread_mutex_lock(&affichage_mutex);
        printf("Philosophe %d mange (repas %d)...\n", philo->id + 1, philo->repas + 1);
        pthread_mutex_unlock(&affichage_mutex);
        sleep(1);

        pthread_mutex_unlock(&baguettes[philo->id]);
        pthread_mutex_unlock(&baguettes[(philo->id + 1) % NB_PHILOSOPHES]);

        philo->repas++;

        pthread_mutex_lock(&affichage_mutex);
        printf("Philosophe %d a terminé de manger %d fois\n", philo->id + 1, philo->repas);
        pthread_mutex_unlock(&affichage_mutex);
    }

    pthread_mutex_lock(&affichage_mutex);
    printf("Philosophe %d a fini de manger 5 fois.\n", philo->id + 1);
    pthread_mutex_unlock(&affichage_mutex);
    pthread_exit(NULL);
}

int main() {
    pthread_t philosophes[NB_PHILOSOPHES];
    Philosophe philo_data[NB_PHILOSOPHES];

    pthread_mutex_init(&affichage_mutex, NULL);
    for (int i = 0; i < NB_PHILOSOPHES; i++) {
        pthread_mutex_init(&baguettes[i], NULL);
    }

    for (int i = 0; i < NB_PHILOSOPHES; i++) {
        philo_data[i].id = i;
        philo_data[i].repas = 0;
        pthread_create(&philosophes[i], NULL, philosopher, &philo_data[i]);
    }

    for (int i = 0; i < NB_PHILOSOPHES; i++) {
        pthread_join(philosophes[i], NULL);
    }

    pthread_mutex_destroy(&affichage_mutex);
    for (int i = 0; i < NB_PHILOSOPHES; i++) {
        pthread_mutex_destroy(&baguettes[i]);
    }

    printf("Tous les philosophes ont terminé leurs repas.\n");
    return 0;
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Fonction exécutée par le thread
void* thread_function(void* arg) {
    pthread_t thread_id = pthread_self(); // Récupère l'ID du thread
    pthread_attr_t attr; // Attributs du thread
    int policy;
    struct sched_param param;

    // Initialise les attributs du thread
    pthread_getattr_np(thread_id, &attr);

    // Affiche l'ID du thread
    printf("Thread ID: %lu\n", thread_id);

    // Récupère et affiche la politique d'ordonnancement
    pthread_attr_getschedpolicy(&attr, &policy);
    if (policy == SCHED_OTHER) {
        printf("Scheduling policy: SCHED_OTHER\n");
    } else if (policy == SCHED_FIFO) {
        printf("Scheduling policy: SCHED_FIFO\n");
    } else if (policy == SCHED_RR) {
        printf("Scheduling policy: SCHED_RR\n");
    }

    // Récupère et affiche la priorité du thread
    pthread_attr_getschedparam(&attr, &param);
    printf("Thread priority: %d\n", param.sched_priority);

    // Récupère et affiche la taille de la pile (stack)
    size_t stacksize;
    pthread_attr_getstacksize(&attr, &stacksize);
    printf("Stack size: %zu bytes\n", stacksize);

    // Libère les attributs
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    int result;

    // Crée un thread avec les attributs par défaut
    result = pthread_create(&thread, NULL, thread_function, NULL);
    if (result != 0) {
        printf("Error creating thread\n");
        return EXIT_FAILURE;
    }

    // Attendre la fin du thread
    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}

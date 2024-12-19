#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h> //Pour les caractère
#include <math.h> //Pour arrondir

void* t1(void* arg) {
    float* number = (float*)arg;
    printf("Je suis le premier thread. Mon TID est %ld\n", pthread_self());
    printf("Le nombre reçu est : %.2f\n", *number);

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        pthread_exit(NULL);
    }

    *result = lrintf(*number);
    pthread_exit(result);
}

void* t2(void* arg) {
    char* str = (char*)arg;
    printf("Je suis le second thread. Mon TID est %ld\n", pthread_self());
    printf("La chaîne reçue est : %s\n", str);

    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }

    char* result = (char*)malloc(strlen(str) + 1);
    if (result == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        pthread_exit(NULL);
    }

    strcpy(result, str);
    pthread_exit(result);
}

int main() {
    pthread_t thread1, thread2;
    float number = 5.78;
    char string[] = "Bonjour";

    if (pthread_create(&thread1, NULL, t1, &number)) {
        fprintf(stderr, "Erreur lors de la création du premier thread\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, t2, string)) {
        fprintf(stderr, "Erreur lors de la création du second thread\n");
        return 1;
    }

    int* result1;
    if (pthread_join(thread1, (void**)&result1)) {
        fprintf(stderr, "Erreur lors de la terminaison du premier thread\n");
        return 1;
    }
    printf("Le premier thread (TID %ld) s'est terminé avec la valeur %d\n", thread1, *result1);
    free(result1);
}

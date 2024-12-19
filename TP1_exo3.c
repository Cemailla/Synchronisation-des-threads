#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Définition des paramètres pour les threads
typedef struct {
    int op1;
    int op2;
    int *result;
    int (*operation)(int, int); // Pointeur sur la fonction d'opération
} thread1_args_t;

typedef struct {
    int op1;
    int op2;
    int (*operation)(int, int); // Pointeur sur la fonction d'opération
} thread2_args_t;

// Fonctions d'opération
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

// Fonction exécutée par le premier thread
void* thread1_function(void* arg) {
    thread1_args_t *args = (thread1_args_t*)arg;
    int *result_ptr = (int*)malloc(sizeof(int)); // Allocation dynamique pour stocker le résultat
    if (result_ptr == NULL) {
        perror("Allocation de mémoire échouée pour le premier thread");
        pthread_exit(NULL);
    }
    *result_ptr = args->operation(args->op1, args->op2);
    pthread_exit(result_ptr); // Retourne le résultat sous forme de pointeur
}

// Fonction exécutée par le second thread
void* thread2_function(void* arg) {
    thread2_args_t *args = (thread2_args_t*)arg;
    int result = args->operation(args->op1, args->op2);
    int *result_ptr = (int*)malloc(sizeof(int)); // Allocation dynamique
    if (result_ptr == NULL) {
        perror("Allocation de mémoire échouée pour le second thread");
        pthread_exit(NULL);
    }
    *result_ptr = result;
    pthread_exit(result_ptr); // Retourne le résultat sous forme de pointeur
}

// Fonction pour lire un entier avec gestion des erreurs
int read_integer(const char *prompt, int *value) {
    char buffer[100];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0; // Erreur de lecture
    }
    char *endptr;
    *value = strtol(buffer, &endptr, 10);
    return (*endptr == '\0' || *endptr == '\n'); // Vérifie si la lecture est réussie
}

int main() {
    pthread_t thread1, thread2;
    int op1, op2;
    char operation;
    void* retval1;
    void* retval2;
    int (*op_func)(int, int) = NULL;

    // Menu pour choisir l'opération
    printf("Choisissez une opération (+, -, *): ");
    scanf(" %c", &operation);
    getchar(); // Consomme le retour à la ligne après le caractère de l'opération

    // Choix de la fonction d'opération
    switch(operation) {
        case '+': op_func = add; break;
        case '-': op_func = sub; break;
        case '*': op_func = mul; break;
        default:
            fprintf(stderr, "Opération non valide.\n");
            return EXIT_FAILURE;
    }

    // Saisie des opérandes avec validation
    if (!read_integer("Entrez la première valeur: ", &op1)) {
        fprintf(stderr, "Erreur: entrée invalide pour la première valeur.\n");
        return EXIT_FAILURE;
    }

    if (!read_integer("Entrez la seconde valeur: ", &op2)) {
        fprintf(stderr, "Erreur: entrée invalide pour la seconde valeur.\n");
        return EXIT_FAILURE;
    }

    // Création des arguments pour les threads
    thread1_args_t args1 = {op1, op2, NULL, op_func};
    thread2_args_t args2 = {op1, op2, op_func};

    // Création du premier thread
    if (pthread_create(&thread1, NULL, thread1_function, &args1) != 0) {
        perror("Erreur lors de la création du premier thread");
        return EXIT_FAILURE;
    }

    // Création du second thread
    if (pthread_create(&thread2, NULL, thread2_function, &args2) != 0) {
        perror("Erreur lors de la création du second thread");
        return EXIT_FAILURE;
    }

    // Attente de la terminaison du premier thread
    if (pthread_join(thread1, &retval1) != 0) {
        perror("Erreur lors de l'attente du premier thread");
        return EXIT_FAILURE;
    }
    int result1 = *((int*)retval1); // Déférencement du pointeur
    printf("Le premier thread s'est terminé. TID: %lu, Résultat: %d\n",
           (unsigned long)thread1, result1);
    free(retval1); // Libération de la mémoire allouée

    // Attente de la terminaison du second thread
    if (pthread_join(thread2, &retval2) != 0) {
        perror("Erreur lors de l'attente du second thread");
        return EXIT_FAILURE;
    }
    int result2 = *((int*)retval2); // Déférencement du pointeur
    printf("Le second thread s'est terminé. TID: %lu, Résultat: %d\n",
           (unsigned long)thread2, result2);
    free(retval2); // Libération de la mémoire allouée

    return EXIT_SUCCESS;
}

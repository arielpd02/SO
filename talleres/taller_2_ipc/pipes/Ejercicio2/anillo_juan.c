#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include "constants.h"

enum { READ, WRITE };
int (*pipes)[2];
int start;

void close_except(int idx, int count) {
	// Cierra todos los extremos de escritura salvo el
	// indicado por idx.
	for (int p = 0; p < count; p++) {
		if (p != idx) close(pipes[p][WRITE]);
	}
}

void close_start(int count) {
	for (int i = 0; i < count; i++) {
		if (i != (start + 1) % count && i != start) close(pipes[i][WRITE]);
	}
}

int generate_random_number() {
	return (rand() % 50);
}

void child(int p, int n) {
	int msg;
	int r = generate_random_number();

	if (p == start) {
		close_start(n);
	} else {
		close_except((p + 1) % n, n);
	}

	while (read(pipes[p][READ], &msg, sizeof(msg)) > 0) {
		if (p == start && msg >= r) {
			printf("[%d] Read: %d, secret: %d\n", p, msg, r);
			write(pipes[p][WRITE], &msg, sizeof(msg));
			break;
		}
		printf("[%d] Read: %d, write: %d\n", p, msg, msg + 1);
		msg++;
		write(pipes[(p + 1) % n][WRITE], &msg, sizeof(msg));
	}

	close(pipes[(p + 1) % n][WRITE]);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, n, msg;
	n = atoi(argv[1]);
	msg = atoi(argv[2]);
	start = atoi(argv[3]);
	pid_t pids[n];

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, msg, start);
    
    // Creamos pipes.
	pipes = calloc(n, sizeof(int) * 2);
    for (int i = 0; i < n; i++) {
		pipe(pipes[i]);
	}

	// Creamos procesos
	for (int p = 0; p < n; p++) {
		pids[p] = fork();
		if (pids[p] == 0) child(p, n);
	}

	// Envio el mensaje inicial
	write(pipes[start][WRITE], &msg, sizeof(msg));

	// Espero al inicial y leo el valor
	for (int p = 0; p < n; p++) close(pipes[p][WRITE]);
	for (int p = 0; p < n; p++) wait(NULL);

	read(pipes[start][READ], &msg, sizeof(msg));

	printf("El valor final es: %d\n", msg);
	free(pipes);
	exit(EXIT_SUCCESS);
}

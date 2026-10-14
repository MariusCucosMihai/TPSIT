
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {

    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(-1);
    }

    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(-1);
    }

    pid_t ritornoB = fork();

    if (ritornoB == -1) {
        perror("fork");
        exit(-2);
    }

    if (!ritornoB) {

        close(pipe1[1]);
        close(pipe2[0]);

        int n;

        while (read(pipe1[0], &n, sizeof(n)) > 0) {

            if (n >= 50) {
                n = n * n;
                write(pipe2[1], &n, sizeof(n));
            }
        }

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    pid_t ritornoC = fork();

    if (ritornoC == -1) {
        perror("fork");
        exit(-3);
    }

    if (!ritornoC) {

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        int n;
        int somma = 0;
        int contatore = 0;

        while (read(pipe2[0], &n, sizeof(n)) > 0) {

            printf(" numero ricevuto: %d\n", n);

            somma += n;
            contatore++;
        }

        if (contatore > 0) {
            printf(" media: %.2f\n", (float)somma / contatore);
        } else {
            printf("nesun numero ricevuto\n");
        }

        close(pipe2[0]);

        exit(0);
    }

    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);

    srand(time(NULL));

    for (int i = 0; i < 20; i++) {

        int n = rand() % 100 + 1;

        printf(" numero generato: %d\n", n);

        write(pipe1[1], &n, sizeof(n));
    }

    close(pipe1[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}

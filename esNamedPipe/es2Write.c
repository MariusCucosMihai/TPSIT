#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define FIFO_PATH "/tmp/echo_fifo"
#define BUF_SIZE 256

int main() {
    char buf[BUF_SIZE];

    signal(SIGPIPE, SIG_IGN);

    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(1);
    }

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Errore open");
        exit(1);
    }

    while (1) {
        printf("Messaggio: ");
        if (fgets(buf, BUF_SIZE, stdin) == NULL) {
            strcpy(buf, "QUIT");
        }
        buf[strcspn(buf, "\n")] = '\0';

        if (write(fd, buf, BUF_SIZE) == -1) {
            if (errno == EPIPE)
                fprintf(stderr, "Nessun reader: canale chiuso.\n");
            else
                perror("Errore write");
            break;
        }

        if (strcmp(buf, "QUIT") == 0) {
            break;
        }
    }

    close(fd);
    return 0;
}

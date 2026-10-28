#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define FIFO_A_TO_B "/tmp/fifo_a_to_b"
#define FIFO_B_TO_A "/tmp/fifo_b_to_a"
#define BUF_SIZE 256

int main() {
    char buf[BUF_SIZE];

    signal(SIGPIPE, SIG_IGN);

    if (mkfifo(FIFO_A_TO_B, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo a_to_b");
        exit(1);
    }
    if (mkfifo(FIFO_B_TO_A, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo b_to_a");
        exit(1);
    }

    int fd_read = open(FIFO_A_TO_B, O_RDONLY);
    if (fd_read == -1) {
        perror("Errore open a_to_b");
        exit(1);
    }

    int fd_write = open(FIFO_B_TO_A, O_WRONLY);
    if (fd_write == -1) {
        perror("Errore open b_to_a");
        exit(1);
    }

    while (1) {
        ssize_t n = read(fd_read, buf, BUF_SIZE);
        if (n <= 0) {
            break;
        }
        buf[BUF_SIZE - 1] = '\0';

        if (strcmp(buf, "EXIT") == 0) {
            printf("[B] A ha chiuso la chat.\n");
            break;
        }

        printf("[B] A: %s\n", buf);

        printf("[B] Tu: ");
        if (fgets(buf, BUF_SIZE, stdin) == NULL) {
            strcpy(buf, "EXIT");
        }
        buf[strcspn(buf, "\n")] = '\0';

        if (write(fd_write, buf, BUF_SIZE) == -1) {
            perror("Errore write");
            break;
        }
        if (strcmp(buf, "EXIT") == 0) {
            break;
        }
    }

    close(fd_read);
    close(fd_write);
    unlink(FIFO_A_TO_B);
    unlink(FIFO_B_TO_A);

    return 0;
}

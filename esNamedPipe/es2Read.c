#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_PATH "/tmp/echo_fifo"
#define BUF_SIZE 256

int main() {
    char buf[BUF_SIZE];

    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(1);
    }

    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Errore open");
        exit(1);
    }

    while (1) {
        ssize_t n = read(fd, buf, BUF_SIZE);
        if (n <= 0) {
            break;
        }
        buf[BUF_SIZE - 1] = '\0';

        if (strcmp(buf, "QUIT") == 0) {
            printf("[SERVER] Ricevuto QUIT. Arrivederci!\n");
            break;
        }

        printf("[SERVER] %s\n", buf);
    }

    close(fd);
    unlink(FIFO_PATH);

    return 0;
}

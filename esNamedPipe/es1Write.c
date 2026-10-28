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

    printf("Messaggio: ");
    if (fgets(buf, BUF_SIZE, stdin) == NULL) {
        exit(1);
    }
    buf[strcspn(buf, "\n")] = '\0';

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Errore open");
        exit(1);
    }

    if (write(fd, buf, strlen(buf) + 1) == -1) {
        perror("Errore write");
    }

    close(fd);
    return 0;
}

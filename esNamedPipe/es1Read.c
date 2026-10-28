#include <stdio.h>
#include <stdlib.h>
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

    ssize_t n = read(fd, buf, BUF_SIZE - 1);
    if (n > 0) {
        buf[n] = '\0';
        printf("[SERVER] %s\n", buf);
    }

    close(fd);
    unlink(FIFO_PATH);

    return 0;
}

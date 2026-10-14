#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

struct Messaggio {
	char stringa[100];
	int vocali;
};

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

	pid_t ritorno = fork();

	if (ritorno == -1) {
		perror("fork");
		exit(-2);
	}

	if (!ritorno) {

		close(pipe1[1]);
		close(pipe2[0]);

		char buffer[100];

		int bytesLetti = read(pipe1[0], buffer, sizeof(buffer));

		if (bytesLetti <= 0) {
			close(pipe1[0]);
			close(pipe2[1]);
			exit(1);
		}

		struct Messaggio msg;
		int vocali = 0;

		for (int i = 0; buffer[i] != '\0'; i++) {

			buffer[i] = toupper((unsigned char)buffer[i]);

			if (buffer[i] == 'A' || buffer[i] == 'E' ||
			        buffer[i] == 'I' || buffer[i] == 'O' ||
			        buffer[i] == 'U') {
				vocali++;
			}
		}

		strcpy(msg.stringa, buffer);
		msg.vocali = vocali;

		write(pipe2[1], &msg, sizeof(msg));

		close(pipe1[0]);
		close(pipe2[1]);

		exit(0);
	}

	close(pipe1[0]);
	close(pipe2[1]);

	char testo[100];

	printf(" inserisci una stringa: ");
	fgets(testo, sizeof(testo), stdin);

	write(pipe1[1], testo, strlen(testo) + 1);

	close(pipe1[1]);

	struct Messaggio risultato;

	read(pipe2[0], &risultato, sizeof(risultato));

	close(pipe2[0]);

	wait(NULL);

	printf(" stringa maiuscola: %s\n", risultato.stringa);
	printf(" numero di vocali: %d\n", risultato.vocali);

	return 0;
}

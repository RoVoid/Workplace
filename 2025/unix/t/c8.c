#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
	char pathname[] = "c7.c";
	key_t key;

	int semid, shmid;
	struct sembuf sbuf;

	char *textbuf;

	if ((key = ftok(pathname, 0)) < 0) return 1;

	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) return 1;
	if ((shmid = shmget(key, 10, 0666 | IPC_CREAT)) < 0) return 1;

	if (semctl(semid, 0, SETVAL, 0) < 0) return 1;

	textbuf = (char *)shmat(shmid, NULL, 0);
	if (textbuf == (char *)-1) return 1;

	if (fork()) {
		while (1) {
			scanf("%9s", textbuf);
			printf("[Parent]: %s\n", textbuf);

			sbuf.sem_num = 0;
			sbuf.sem_op = 1;
			sbuf.sem_flg = 0;
			semop(semid, &sbuf, 1);

			sbuf.sem_op = -1;
			semop(semid, &sbuf, 1);

			if (strcmp(textbuf, "exit") == 0) break;
		}

		wait(NULL);

		shmdt(textbuf);
		shmctl(shmid, IPC_RMID, NULL);
		semctl(semid, 0, IPC_RMID);
	}
	else {
		int exit = 0;
		while (1) {
			sbuf.sem_num = 0;
			sbuf.sem_op = -1;
			sbuf.sem_flg = 0;
			semop(semid, &sbuf, 1);

			if (strcmp(textbuf, "exit") == 0 && exit < 2) {
				printf("[Child]: Exit? (%d/3)\n", ++exit);
				strcpy(textbuf, "no-exit");
			}
			else {
				printf("[Child]: \"%s\"\n", textbuf);
			}

			sbuf.sem_op = 1;
			semop(semid, &sbuf, 1);

			if (strcmp(textbuf, "exit") == 0) break;
		}

		shmdt(textbuf);
	}

	return 0;
}
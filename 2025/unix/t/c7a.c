#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
int main() {
	int semid;
	char pathname[] = "c7.c";
	key_t key; /* IPC ключ */
	struct sembuf buf;
	if ((key = ftok(pathname, 0) ) < 0) {
		printf("Can\'t generate key\n");
		return 1;
	}

	if ((semid = semget (key, 1, 0666 | IPC_CREAT) ) < 0) {
		printf("Can\'t get semid\n");
		return 1;
	}

	buf.sem_op = 1;
	buf.sem_flg = 0;
	buf.sem_num = 0;
	if (semop(semid, &buf, 1) < 0) {
		printf("Can\'t wait for condition\n");
		return 1;
	}

	printf("Condition is present\n");
	return 0;

}
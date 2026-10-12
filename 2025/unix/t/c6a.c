#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define EEXIST      17  /* File exists */


int main() {
	int shmid;
	char pathname[] = "aa.txt";
	key_t key;
	if ((key = ftok(pathname, 0)) < 0) return -1;

	char *buffer;
	int new = 1;

	if ((shmid = shmget(key, 3 * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
		if (errno != EEXIST) return -1;
		if ((shmid = shmget(key, 3 * sizeof(char), IPC_RMID)) < 0) return -1;
		new = 0;
	}

	buffer = (char*) shmat(shmid, NULL, 0);
	if (new) {
		buffer[0] = 0;
		buffer[1] = 1;
		buffer[2] = 1;
	}
	else {
		buffer[1] += 1;
		for (int i = 0; i < 1000000000; ++i) ;

		// buffer[1] = 0;
		buffer[2] += 1;
	}

	printf("2: Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", buffer [0], buffer [1], buffer [2]);


	if (shmdt (buffer) < 0) {
		printf("Can't detach shared memory\n");
		return -1;
	}
}


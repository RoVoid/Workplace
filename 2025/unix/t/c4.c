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

	if ((shmid = shmget(key, 1024 * sizeof(char), IPC_RMID)) < 0) return -1;


	buffer = (char*) shmat(shmid, NULL, 0);
	printf("%s\n", buffer);



	if (shmdt (buffer) < 0) {
		printf("Can't detach shared memory\n");
		return -1;
	}
}


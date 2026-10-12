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


	int fd = open("c3.c", O_RDONLY);
	char *buffer;

	if ((shmid = shmget(key, 1024 * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
		if (errno != EEXIST) return -1;
		if ((shmid = shmget(key, 1024 * sizeof(char), IPC_RMID)) < 0) return -1;
	}

	int total = 0, bytesRead;

	buffer = (char*) shmat(shmid, NULL, 0);

	while ((bytesRead = read(fd, buffer + total, sizeof(buffer)))) {
		total += bytesRead;
	}
	printf("%d\n", total);
	close(fd);



	if (shmdt (buffer) < 0) {
		printf("Can't detach shared memory\n");
		return -1;
	}
}


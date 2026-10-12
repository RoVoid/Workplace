
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define _S_IFIFO  0x1000

int main() {
	umask(0);
	int result = mkfifo("my.fifo", 0777);

	if (result == -1) {
		perror("mknod failed");
		return 1;
	}


	printf("FIFO created successfully.\n");
	return 0;
}
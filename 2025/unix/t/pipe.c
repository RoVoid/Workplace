#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
// #include <sys/stat.h>

int main() {
	char buf[14] = "";

	int pc[2];
	int cp[2];
	if (pipe(pc) < 0) return -1;
	if (pipe(cp) < 0) return -1;

	if (fork()) {
		close(pc[0]);
		if (write(pc[1], "Hello, World!\n", 14) != 14) return -1;
		printf("[Parent]: Send text\n");
		close(pc[1]);

		close(cp[1]);
		if (read(cp[0], buf, 5) != 5) return -2;
		printf("[Parent]: %s\n", buf);
		close(cp[0]);
	}

	else {
		close(pc[1]);
		if (read(pc[0], buf, 14) != 14) return -2;
		printf("[Client]: %s", buf);
		close(pc[0]);

		close(cp[0]);
		if (write(cp[1], "What?\n", 5) != 5) return -1;
		printf("[Client]: Send text\n");
		close(cp[1]);
	}

	return 0;
}
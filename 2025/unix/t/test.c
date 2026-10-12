#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	int pipefd[2];
	pipe(pipefd);

	// Получить текущий размер
	int size = fcntl(pipefd[1], F_GETPIPE_SZ);
	printf("Размер пайпа: %d байт\n", size);

	// Установить новый размер (требует прав root или ограничения sysctl)
	// fcntl(pipefd[1], F_SETPIPE_SZ, 16384);

	close(pipefd[0]);
	close(pipefd[1]);
	return 0;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
	int sockfd;
	struct sockaddr_in servaddr;
	char sendbuf[1000], recvbuf[1000];
	int n;

	// FIX: проверяем socket()
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("[ERROR] socket");
		return 1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(51000);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	// FIX: проверяем connect()
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		perror("[ERROR] connect");
		close(sockfd);
		return 1;
	}

	printf("Enter message: ");
	fflush(stdout);
	fgets(sendbuf, sizeof(sendbuf), stdin);

	int len = strlen(sendbuf);

	// FIX: гарантированная отправка (как на сервере)
	int total = 0;
	while (total < len) {
		int sent = write(sockfd, sendbuf + total, len - total);
		if (sent < 0) {
			perror("[ERROR] write");
			close(sockfd);
			return 1;
		}
		total += sent;
	}

	printf("[INFO] Sent %d bytes\n", total);

	// FIX: гарантированное получение — читаем ровно столько, сколько отправили
	int received = 0;
	while (received < len) {
		n = read(sockfd, recvbuf + received, len - received);
		if (n <= 0) {
			if (n < 0) perror("[ERROR] read");
			else printf("[INFO] Server closed connection early\n");
			break;
		}
		received += n;
	}

	if (received > 0) {
		printf("echo: ");
		fwrite(recvbuf, 1, received, stdout);
		// fgets сохраняет \n, поэтому дополнительный \n не нужен
	}

	close(sockfd);
	return 0;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
	int sockfd, newsockfd;
	socklen_t clilen;
	int n;
	char buf[1000];
	struct sockaddr_in servaddr, cliaddr;

	printf("[INFO] Starting server...\n");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("[ERROR] socket");
		return 1;
	}

	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	printf("[INFO] Socket created: fd=%d\n", sockfd);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_port        = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		perror("[ERROR] bind");
		close(sockfd);
		return 1;
	}

	printf("[INFO] Bound to port 51000\n");

	if (listen(sockfd, 5) < 0) {
		perror("[ERROR] listen");
		close(sockfd);
		return 1;
	}

	printf("[INFO] Listening...\n");

	while (1) {
		clilen = sizeof(cliaddr);

		printf("[INFO] Waiting for connection...\n");

		if ((newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen)) < 0) {
			perror("[ERROR] accept");
			continue;
		}

		printf("[INFO] Client connected: %s:%d (fd=%d)\n",
		    inet_ntoa(cliaddr.sin_addr),
		    ntohs(cliaddr.sin_port),
		    newsockfd);

		while ((n = read(newsockfd, buf, sizeof(buf))) > 0) {
			printf("[INFO] Received %d bytes\n", n);
			printf("[DATA] ");
			fwrite(buf, 1, n, stdout);
			printf("\n");

			int total = 0;
			int error = 0;
			while (total < n) {
				int sent = write(newsockfd, buf + total, n - total);
				if (sent < 0) {
					perror("[ERROR] write");
					error = 1;
					break;
				}
				total += sent;
			}

			printf("[INFO] Sent back %d bytes\n", total);

			// FIX: выходим из внешнего цикла при ошибке записи
			if (error) break;
		}

		if (n == 0)
			printf("[INFO] Client disconnected (fd=%d)\n", newsockfd);
		else if (n < 0)
			perror("[ERROR] read");

		close(newsockfd);
		printf("[INFO] Connection closed\n");
	}

	close(sockfd);
	return 0;
}

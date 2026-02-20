#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd_in, fd_out;
    char buf[1024];

    fd_in  = open("test.txt", O_RDONLY);
    fd_out = open("test_out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd_in < 0 || fd_out < 0) {
        perror("open");
        return 1;
    }

    fork();
    fork();

    int n;
    while ((n = read(fd_in, buf, 2)) > 0) {
        buf[n] = '\0';

        printf("%d: %s\n", getpid(), buf);

        n = sprintf(buf, "%d: Hello, world!\n", getpid());
        write(fd_out, buf, n);

        sleep(1);
    }

    close(fd_in);
    close(fd_out);
    return 0;
}

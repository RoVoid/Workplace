#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
	int fd;
	size_t length;
	int i;

	// struct A {
	// 	double f;
	// 	double f2;
	// }*ptr, *tmpptr;


	fd = open ("mapped.dat", O_RDWR | O_CREAT, 0666);
	if ( fd == -1) {
		printf("File open failed!\n");
		return 1;
	}

	// length = 100000 * sizeof(struct A);
	length = sizeof(long long);

	ftruncate(fd, length);
	// ptr = (struct A *) mmap (NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	long long *ptr = (long long*) mmap (NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

	close(fd);
	if (ptr == MAP_FAILED) {
		printf("Mapping failed!\n");
		return 2;
	}

	// tmpptr = ptr;
	// for (i = 1; i <= 100000; i++) {
	// 	tmpptr->f = i;
	// 	tmpptr->f2 = tmpptr->f * tmpptr->f;
	// 	tmpptr++;
	// }

	long long *t = ptr;
	*t = 0;
	for (i = 1; i <= 10; i++) {
		*t += i;
	}

	printf("%lld\n", *t);


	munmap((void *)ptr, length);


	return 0;
}
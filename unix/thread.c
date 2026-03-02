#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* func(void *arg) {
	static int used = 0;
	++used;

	int id = *(int*)arg;

	pid_t tid = gettid();

	printf("Thread id: %d  LWP id: %d  Using count: %d\n\n", id, tid, used);

	int *r = malloc(sizeof(int));
	*r = id * id;

	return r;
}

int main() {
	pthread_t threads[10];
	int ids[10];

	for (int i = 0; i < 10; ++i) {
		ids[i] = i;
		pthread_create(&threads[i], NULL, func, &ids[i]);
	}

	sleep(1);

	void *r;

	for (int i = 0; i < 10; ++i) {
		pthread_join(threads[i], &r);

		printf("Return: %d\n", *(int*)r);
		free(r);
	}

	return 0;
}
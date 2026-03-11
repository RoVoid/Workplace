#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define N 5
#define T 10

pthread_mutex_t forks[N];
int eaten[N];

volatile int running = 1;

void* philosopher(void* arg) {
	int id = *(int*)arg;

	int left = id;
	int right = (id + 1) % N;

	if (left > right) {
		int tmp = left;
		left = right;
		right = tmp;
	}

	while (running) {

		pthread_mutex_lock(forks + left);
		pthread_mutex_lock(forks + right);

		if (!running) {
			pthread_mutex_unlock(forks + left);
			pthread_mutex_unlock(forks + right);
			break;
		}

		printf("Philosopher %d eating\n", id);
		eaten[id]++;

		sleep(rand() % 3 + 1);

		pthread_mutex_unlock(forks + left);
		pthread_mutex_unlock(forks + right);

		sleep(1);
	}

	return NULL;
}

int main() {
	pthread_t philosophers[N];
	int ids[N];

	srand(time(NULL));

	for (int i = 0; i < N; i++) pthread_mutex_init(forks + i, NULL);

	for (int i = 0; i < N; i++) {
		ids[i] = i;
		pthread_create(philosophers + i, NULL, philosopher, &ids[i]);
	}

	sleep(T);
	running = 0;

	for (int i = 0; i < N; i++) pthread_join(philosophers[i], NULL);

	printf("\nResult:\n");
	for (int i = 0; i < N; i++) printf("Philosopher %d -> %d\n", i, eaten[i]);

	return 0;
}
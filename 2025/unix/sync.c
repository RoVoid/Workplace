#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 3 // Пекари
#define M 5 // Покупатели
#define BUFFER_SIZE 2 // число мест

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int r = 10;
int b = 0;

void* baker(void* arg) {
	int id = *(int*)arg;
	while (r > 0) {
		sem_wait(&empty); // ожидание места
		pthread_mutex_lock(&mutex);

		if (r > 0) printf("+%d, Пекарь %d: испек булку! (Осталось на %d)\n", ++b, id, --r);

		pthread_mutex_unlock(&mutex);
		sem_post(&full); // +еда
		sleep(1);
	}

	printf("Пекарь %d: уходит\n", id);

	return NULL;
}

void* customer(void* arg) {
	int id = *(int*)arg;
	while (r > 0) {
		sem_wait(&full); // ожидание булки
		pthread_mutex_lock(&mutex);

		if (b > 0) printf("-%d, Покупатель %d: купил булку!\n", --b, id);

		pthread_mutex_unlock(&mutex);
		sem_post(&empty); // освобождаем место
		sleep(2);
	}

	printf("Покупатель %d: уходит\n", id);

	return NULL;
}

int main() {
	pthread_t bTh[N], cTh[M];
	int bIds[N], cIds[M];

	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);

	for (int i = 0; i < N; i++) {
		bIds[i] = i + 1;
		pthread_create(&bTh[i], NULL, baker, &bIds[i]);
	}
	for (int i = 0; i < M; i++) {
		cIds[i] = i + 1;
		pthread_create(&cTh[i], NULL, customer, &cIds[i]);
	}

	for (int i = 0; i < N; i++) pthread_join(bTh[i], NULL);
	for (int i = 0; i < M; i++) pthread_join(cTh[i], NULL);

	return 0;
}

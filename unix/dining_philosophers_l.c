#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define N 5
#define T 100

pthread_mutex_t forks[N];
int eaten[N];

int running = 1;

void* philosopher(void* arg) {
    int id = *(int*)arg;

    int left = id;
    int right = (id + 1) % N;

	// if(left > right){
	// 	int tmp = right;
	// 	right = left;
	// 	left = tmp;
	// }

    while (running) {
		int think_time = id + 1;
        printf("Philosopher %d is thinking for %d sec\n", id, think_time);
        sleep(think_time);

        printf("Philosopher %d wants to eat\n", id);

        int attempts = 0;

		pthread_mutex_lock(forks + left);
        printf("Philosopher %d took fork %d\n", id, left);

        while (attempts < 3) {
			sleep(1);
            if (pthread_mutex_trylock(forks + right) == 0) {
                printf("Philosopher %d took fork %d\n", id, right);
                break;
            } 
            printf("Philosopher %d: fork %d busy (Fails: %d)\n", id, right, ++attempts);
            if(attempts == 3) {
				printf("Philosopher %d put fork %d\n", id, left);
				pthread_mutex_unlock(forks + left);
				pthread_mutex_lock(forks + right);
				printf("Philosopher %d took fork %d\n", id, right);
			}
        }

		while (attempts < 6 && attempts >= 3) {
			sleep(1);
            if (pthread_mutex_trylock(forks + left) == 0) {
                printf("Philosopher %d took fork %d\n", id, left);
                break;
            } 
            printf("Philosopher %d: fork %d busy (Fails*: %d)\n", id, left, ++attempts - 3);
            if(attempts == 6) {
				printf("Philosopher %d put fork %d\n", id, right);
				pthread_mutex_unlock(forks + right);
			}
        }

		if(attempts == 6) {
			printf("Philosopher %d left\n", id);
			break;
		}


        if (!running) {
			printf("Philosopher %d put forks %d & %d\n", id, left, right);
            pthread_mutex_unlock(forks + left);
            pthread_mutex_unlock(forks + right);
            break;
        }

        int eat_time = id == 1 ? 20 : id + 1;
        printf("Philosopher %d is eating for %d sec\n", id, eat_time);
        eaten[id]++;
        sleep(eat_time);

        printf("Philosopher %d finished eating\n", id);
		printf("Philosopher %d put forks %d & %d\n", id, left, right);
		pthread_mutex_unlock(forks + left);
        pthread_mutex_unlock(forks + right);
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

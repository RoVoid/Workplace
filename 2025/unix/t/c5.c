
#include <pthread.h>
#include <stdio.h>
int a = 0;

void *mythread (void *dummy) {
	pthread_t mythid;

	mythid = pthread_self();
	a = a + 1;

	printf("Thread %lu, Calculation result = %d\n", mythid, a);
	return NULL;
}

int main() {
	pthread_t th_id, my_th_id;
	int result;

	result = pthread_create(&th_id, (pthread_attr_t *) NULL, mythread, NULL);
	if (result != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		return -1;
	}
	result = pthread_create(&th_id, (pthread_attr_t *) NULL, mythread, NULL);
	if (result != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		return -1;
	}
	printf("Thread created, th_id = %lu\n", th_id);
	my_th_id = pthread_self();
	a = a + 1;
	printf("Thread %lu, Calculation result = %d\n", my_th_id, a);
	pthread_join(th_id, (void **)NULL);
	return 0;
}
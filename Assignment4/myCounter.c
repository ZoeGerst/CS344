



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SIZE 2

pthread_cond_t myCond1; //empty
pthread_cond_t myCond2; //full
int myCount = 0;
int buffer[10];
int counter = 0;

pthread_mutex_t myMutex;

void put_item(int value){

	myCount++;
	buffer[myCount] = value;

//	printf("myCount %d -> ", myCount);

}

void get_item(){


	buffer[myCount] = -1;

	myCount++;

//	printf("%d\n", myCount);


}


void *producer(){

	int value = 0;
	printf("CONSUMER THREAD CREATED\n");

	while(myCount < 10){

		pthread_mutex_lock(&myMutex);

		printf("PRODUCER: myMutex is locked\n");

		while (myCount == SIZE){

			pthread_cond_wait(&myCond1, &myMutex);

			printf("PRODUCER: waiting on myCond1\n");

		}
		printf("myCount: %d -> ", myCount);

		put_item(value);

		printf("%d\n", myCount);

		//if(myCount == 10){

		//	return 0;

		//}
//		myCount++;

//		myCount++;

//		printf("myCount: %d -> %d\n", value, myCount);

		pthread_mutex_unlock(&myMutex);

		printf("PRODUCER: myMutex is unlocked\n");

		pthread_cond_signal(&myCond2);

		printf("PRODUCER: signaling myCond2\n");

		counter++;
	}

}


void *consumer(){

	int value;

	while(myCount < 10){

		pthread_mutex_lock(&myMutex);

		printf("CONSUMER: myMutex is locked\n");

		while (myCount == 0){

			pthread_cond_wait(&myCond2, &myMutex);

			printf("CONSUMER: waiting on myCond2\n");

		}

		printf("myCount: %d -> ", myCount);

		get_item();

		printf("%d\n", myCount);

		pthread_mutex_unlock(&myMutex);

		printf("CONSUMER: myMutex is unlocked\n");

		pthread_cond_signal(&myCond1);

		printf("PRODUCER: signaling myCond1\n");

		counter++;
	}

}



int main(){

	srand(time(NULL));

	pthread_t PRO[SIZE];

	int x, y;

	for(int i = 0; i < 10; i++){

		buffer[i] = -1;

	}

	printf("PROGRAM START\n");

	x = pthread_create(&PRO[0], NULL, producer, NULL);
	y = pthread_create(&PRO[1], NULL, consumer, NULL);


	pthread_join(PRO[0], NULL);
	pthread_join(PRO[1], NULL);

	//if(myCount == 10){

	//	return 0;

	//}

	printf("PROGRAM END\n");

	return 0;

}

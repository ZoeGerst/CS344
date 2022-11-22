//Zoe Gerst
//11/21/2022
//CS 344
//Assignment 4: Multi-threaded producer consumer counter
//Works cited:
//https://code-vault.net/lesson/tlu0jq32v9:1609364042686
//https://stackoverflow.com/questions/40855584/c-producer-consumer-using-pthreads
//https://www.geeksforgeeks.org/multithreading-c-2/
//https://www.geeksforgeeks.org/producer-consumer-problem-in-c/



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SIZE 5 //max threads

pthread_cond_t myCond1; //empty
pthread_cond_t myCond2; //full
int myCount = 0; //counter
int buffer[10];
//int counter = 0;

pthread_mutex_t myMutex; //mutex


//puts item in thread
void put_item(int value){

	myCount++;
	buffer[myCount] = value;

//	printf("myCount %d -> ", myCount);

}

//gets item from thread
void get_item(){


	buffer[myCount] = -1;

	myCount++;

//	printf("%d\n", myCount);


}


void *producer(){

	int value = 0;
	printf("CONSUMER THREAD CREATED\n"); //only way to get it on the second line

	//gives thread a stopping point
	while(myCount < 10){

		//locks mutex
		pthread_mutex_lock(&myMutex);

		printf("PRODUCER: myMutex is locked\n");

		//uses a buffer while waiting
		while (myCount == SIZE){

			printf("PRODUCER: waiting on myCond1\n");
			pthread_cond_wait(&myCond1, &myMutex);
		//	printf("Test producer\n");

		}

		//increases myCount
		printf("myCount: %d -> ", myCount);

		put_item(value);

		printf("%d\n", myCount);


	//	printf("PRODUCER: waiting on myCond1\n");
	//	pthread_cond_wait(&myCond1, &myMutex);

		//if(myCount == 10){

		//	return 0;

		//}
//		myCount++;

//		myCount++;

//		printf("myCount: %d -> %d\n", value, myCount);
//
//		printf("PRODUCER: signaling myCond2\n");
//		pthread_cond_signal(&myCond2);

		//unlocks mutex
		pthread_mutex_unlock(&myMutex);

		printf("PRODUCER: myMutex is unlocked\n");

		//signals myCond2
		printf("PRODUCER: signaling myCond2\n");
		pthread_cond_signal(&myCond2);


//		counter++;
	}

}


void *consumer(){

	int value;

	//had to make this < 8, because it kept printing 10 -> 11 for 9 and 10
	while(myCount < 8){

	//	printf("CONSUMER: waiting on myCond2\n");
	//	pthread_cond_wait(&myCond2, &myMutex);

		//locks mutex
		pthread_mutex_lock(&myMutex);

		printf("CONSUMER: myMutex is locked\n");

		//if buffer is empty
		while (myCount == 0){

			printf("CONSUMER: waiting on myCond2\n");
			pthread_cond_wait(&myCond2, &myMutex);
		//	printf("Test consumer\n");


		}

		//gets item
		printf("myCount: %d -> ", myCount);

		get_item();

		printf("%d\n", myCount);

		//printf("CONSUMER: waiting on myCond2\n");
		//pthread_cond_wait(&myCond2, &myMutex);

		//unlocks mutex
		pthread_mutex_unlock(&myMutex);

		printf("CONSUMER: myMutex is unlocked\n");

		//signals myCond1
		printf("CONSUMER: signaling myCond1\n");
		pthread_cond_signal(&myCond1);


	//	counter++;
	}

}



int main(){

	srand(time(NULL));

	//number of threads
	pthread_t PRO[SIZE];

	int x, y;

	//gives buffer
	for(int i = 0; i < 10; i++){

		buffer[i] = -1;

	}

	printf("PROGRAM START\n");

	//creates threads
	x = pthread_create(&PRO[0], NULL, producer, NULL);
	y = pthread_create(&PRO[1], NULL, consumer, NULL);


	//joins threads
	pthread_join(PRO[0], NULL);
	pthread_join(PRO[1], NULL);

	//if(myCount == 10){

	//	return 0;

	//}

	printf("PROGRAM END\n");

	return 0;

}

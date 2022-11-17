



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE 10

int myCond1; //empty
int myCond2; //full
int myCount = 0;
int buffer[10];

pthread_mutex_t myMutex




void producer(){


	while(1){

		pthread_mutex_lock(&myMutex);

		while (myCount == SIZE){

			pthread_cond_wait(&myCond1, &myMutex);

		}

		put_item(value);

		pthread_mutex_unlock(&myMutex);

		pthread_cond_signal(&myCond2); 

	}

}


void consumer(){


	while(1){

		pthread_mutex_lock(&myMutex);

		while (myCount == 0){

			pthread_cond_wait(&myCond2, &myMutex);

		}

		value = get_item();

		pthread_mutex_unlock(&myMutex);

		pthread_cond_signal(&myCond1);

	}

}



int main(){



	printf("PROGRAM START\n");




	printf("PROGRAM END\n");

	return 0;

}

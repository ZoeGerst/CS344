



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>




void producer(){


	while(1){

		pthread_mutex_lock(&mutex);

		while (count == SIZE){

			pthread_cond_wait(&space_available, &mutex);

		}

		put_item(value);

		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&has_data); 

	}

}


void consumer(){


	while(1){

		pthread_mutex_lock(&mutex);

		while (count == 0){

			pthread_cond_wait(&has_data, &mutex);

		}

		value = get_item();

		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&space_available);

	}

}



int main(){



}

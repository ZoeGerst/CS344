


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main(int ardc, char argv[]){



	if(argc != 2){

		

	}

	int keylength = atoi(argv[1]);
	char randArr;

	if(keylength <= 0){

		

	}

	srand(time(NULL));

	for(int i = 0; i < keylength; i++){

		randArr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ "[rand() % 27];

	}



	return 0;
}

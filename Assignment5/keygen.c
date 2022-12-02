


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

		fprintf(stderr, "Correct length of key is not given\n");
		exit(0);

	}

	int keylength = atoi(argv[1]);
	char finalKey[atoi(argv[1])];
	memset(finalKey, '\0', sizeof(finalKey));
	char randArr;

	if(keylength <= 0){

		fprintf(stderr, "Keylength must be greater than 0\n");
		exit(0);

	}

	srand(time(NULL));

	for(int i = 0; i < keylength; i++){

		randArr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ "[rand() % 27];
		finalKey[i] = randArr;

	}

	finalKey[keylength] = '\0';

	printf("%s\n", finalKey);

	return 0;
}

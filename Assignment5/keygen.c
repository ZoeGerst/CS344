//Zoe Gerst
//12/7/2022
//CS344
//Assignment 5: One-Time Pads: keygen.c
////Works Cited:
//https://replit.com/@cs344/81dnsbyname#main.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main(int argc, char* argv[]){


	//Must be the correct length
	if(argc != 2){

		fprintf(stderr, "Correct length of key is not given\n");
		exit(0);

	}

//	char *pointer;
//	pointer = &argv[1];
//
//	user input

	int keylength = atoi(argv[1]);
	char finalKey[atoi(argv[1])];
	memset(finalKey, '\0', sizeof(finalKey));
	char randArr;

	if(keylength <= 0){

	//	printf("keylength is %d\n", argc);
	//	printf("argv %s\n", argv);
		fprintf(stderr, "Keylength must be greater than 0\n");
		exit(0);

	}

	//random time
	srand(time(NULL));

	//creates a random set of characters
	for(int i = 0; i < keylength; i++){

		randArr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ "[rand() % 27];
		finalKey[i] = randArr;

	}

	finalKey[keylength] = '\0';

	printf("%s\n", finalKey);

	return 0;
}

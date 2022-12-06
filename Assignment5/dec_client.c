


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

#define SIZE 150000

// Error function used for reporting issues
void error(const char *msg) { 
  perror(msg); 
  exit(0); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber, 
                        char* hostname){
 
  // Clear out the address struct
  	memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  	address->sin_family = AF_INET;
  // Store the port number
  	address->sin_port = htons(portNumber);

  // Get the DNS entry for this host name
  	struct hostent* hostInfo = gethostbyname(hostname); 
  	if (hostInfo == NULL) { 
    	fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
    	exit(0); 
  	}
  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  	memcpy((char*) &address->sin_addr.s_addr, 
        	hostInfo->h_addr_list[0],
        	hostInfo->h_length);
}

int main(int argc, char *argv[]) {

 	int socketFD, portNumber, charsWritten, charsRead;
  	struct sockaddr_in serverAddress;
  	char buffer[SIZE];
	char finalKey[SIZE];
	FILE *ciphertext;
	FILE *keyFile;


  // Check usage & args
  	if (argc < 4) {
 
    	fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); 
    	exit(0); 
  	} 

  // Create a socket
  	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
  	
	if (socketFD < 0){
    	
		error("CLIENT: ERROR opening socket");
  	}

   // Set up the server address struct
  	setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

  // Connect to server
  	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
    	
		close(socketFD);
		error("CLIENT: ERROR connecting");
  	}

  // Get input message from user

 // 	printf("CLIENT: Enter text to send to the server, and then hit enter: ");
  

	char *pName = "dec_client";

	if(send(socketFD, pName, strlen(pName), 0) < 0){

		fprintf(stderr, "CLIENT: ERROR writing to socket\n");
		close(socketFD);
		exit(0);

	}

	char sockErr[200];

	if(recv(socketFD, sockErr, sizeof(sockErr), 0) < 0){

		fprintf(stderr, "CLIENT: ERROR reading socket\n");
		close(socketFD);
		exit(0);

	}

	if(strstr(sockErr, "reject") != NULL){

		fprintf(stderr, "CLIENT: ERROR dec_server denied connection on port %d\n", atoi(argv[3]));
		close(socketFD);
		exit(1);

	}

	ciphertext = fopen(argv[1], "r");
	if(ciphertext == NULL){

		close(socketFD);
		fprintf(stderr, "CLIENT: ERROR opening %s file\n", argv[1]);
		exit(0);
		
	}


	// Clear out the buffer array
  	memset(buffer, '\0', sizeof(buffer));

	fscanf(ciphertext, "%[^\n]", buffer);

	int wrong;
	fprintf(stderr, "%s\n", buffer);
	for(int i = 0; i < strlen(buffer); i++){

		char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
		wrong = 0;

		for(int j = 0; j < 27; j++){

			if(buffer[i] != str[j]){

				wrong++;

			}

			

		}
		if(wrong > 26){

			close(socketFD);
			fprintf(stderr, "buffer %d\n", buffer[i]);
			fprintf(stderr, "DEC_CLIENT: ERROR input contains bad characters\n");
			exit(1);

		}

	}


	fclose(ciphertext);

	keyFile = fopen(argv[2], "r");

	if(keyFile == NULL){

		close(socketFD);
		fprintf(stderr, "CLIENT: ERROR %s file\n", argv[1]);
		exit(1);

	}

	memset(finalKey, '\0', sizeof(finalKey));

	fscanf(keyFile, "%[^\n]", finalKey);

	int wrongK;
	for(int i = 0; i < strlen(finalKey); i++){

		char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
		wrongK = 0;

		for(int j = 0; j < 27; j++){

			if(finalKey[i] != str[j]){

				wrongK++;

			}

		}

		if(wrongK > 26){

			close(socketFD);
			printf("buffer %d\n", buffer[i]);
			fprintf(stderr, "DEC_CLIENT: ERROR input contains bad characters\n");
			exit(1);

		}

	}

	fclose(keyFile);

	if(strlen(finalKey) < strlen(buffer)){

		close(socketFD);
		fprintf(stderr, "CLIENT: ERROR key '%s' is too short\n", argv[2]);
		exit(1);

	}

	
	

  // Get input from the user, trunc to buffer - 1 chars, leaving \0
  
//	fgets(buffer, sizeof(buffer) - 1, stdin);
  // Remove the trailing \n that fgets adds
  
	//buffer[strcspn(buffer, "\n")] = '\0'; 

  // Send message to server
  // Write to the server
  

	int pSize = strlen(buffer);

	charsWritten = send(socketFD, &pSize, sizeof(pSize), 0);

	if (charsWritten < 0){

		error("CLIENT: ERROR writing to socket");

	}

	charsWritten = send(socketFD, buffer, strlen(buffer), 0); 
  
	if (charsWritten < 0){
    
		error("CLIENT: ERROR writing to socket");
		
  	}
  
	if (charsWritten < strlen(buffer)){
    
		printf("CLIENT: WARNING: Not all data written to socket!\n");
  	}

	charsWritten = send(socketFD, finalKey, strlen(finalKey), 0);

	if (charsWritten < 0){

		error("CLIENT: ERROR writing to socket");

	}

	if (charsWritten < strlen(buffer)){

		printf("CLIENT: WARNING: Not all data written to socket!\n");

	}

  // Get return message from server
  // Clear out the buffer again for reuse
  
	memset(buffer, '\0', sizeof(buffer));
  // Read data from the socket, leaving \0 at end
  
	charsRead = recv(socketFD, buffer, pSize, 0); 
  
	if (charsRead < 0){
    
		error("DEC_CLIENT: ERROR reading from socket");
  	}
  
	printf("DEC_CLIENT: I received this from the server: \"%s\"\n", buffer);

  // Close the socket
  
	close(socketFD); 
  
	return 0;
}

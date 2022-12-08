//Zoe Gerst
//12/7/2022
//CS344
//Assignment 5: One-Time Pads: dec_server.c
//Works Cited:
//https://replit.com/@cs344/83serverc?lite=true#server.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>



#define SIZE 150000

// Error function used for reporting issues
void error(const char *msg) {
  perror(msg);
  exit(1);
} 

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}

void decryption(int connectionSocket, char* buffer, char* finalKey, char* ciphText, char* clientAccepted, char* clientRejected){

	int firstWord;
	int secondWord;
	int decryptSum;
	int charWritten;
	int charRead;
	int readSocket = 0;
	char clientP[25];

	memset(clientP, '\0', sizeof(clientP));

	//gets ciphertexts from dec_client
	charRead = recv(connectionSocket, clientP, sizeof(clientP), 0);

	if(charRead < 0){


		//error messages
		error("SERVER: ERROR reading socket");

	}
	if(strstr(clientP, "dec_client") != NULL){

		charWritten = send(connectionSocket, clientAccepted, sizeof(clientAccepted), 0);
		if(charWritten < 0){

			error("SERVER: ERROR writing socket");

		}

	}
	else{

		charWritten = send(connectionSocket, clientRejected, sizeof(clientRejected), 0);

		if(charWritten < 0){

			error("SERVER: ERROR writing socket");
		}

		fprintf(stderr, "SERVER: unexpected program\n");
		exit(1);

	}

	charRead = recv(connectionSocket, &readSocket, sizeof(readSocket), 0);
	if(charRead < 0){

		error("SERVER: ERROR reading socket");

	}

	//gets size of file
	memset(ciphText, '\0', sizeof(ciphText));
//	memset(buffer, '\0', sizeof(finalKey));

	charRead = recv(connectionSocket, ciphText, readSocket, 0);
//	charRead = recv(connectionSocket, buffer, readSocket, 0);

	if(charRead < 0){

		error("SERVER: ERROR reading socket");

	}


	memset(finalKey, '\0', sizeof(finalKey));
	charRead = recv(connectionSocket, finalKey, readSocket, 0);

	if(charRead < 0){

		error("SERVER: ERROR reading socket");

	}

	memset(buffer, '\0', sizeof(buffer));
	
	

	//the actual decrypting process
	for(int i = 0; i < strlen(ciphText); i++){

		//allows sum to start from the beginning when counting
		decryptSum = 0;
		char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";


		for(int j = 0; j < 27; j++){

			if(ciphText[i] == alpha[j]){

				firstWord = j;

			}

			if(finalKey[i] == alpha[j]){

				secondWord = j;

			}
			

		}

		//decryption algorithm 
		decryptSum = firstWord - secondWord;

		if(decryptSum < 0){
	
			decryptSum += 27;

		}

		//char arrSpace[] = " ";
		//if(alpha[decryptSum] == arrSpace){

		//	buffer[i] = arrSpace;

		//}
		//else{
		//
		//stores decryption in buffer
		buffer[i] = alpha[decryptSum];
	//charWritten = send(connectionSocket, buffer, readSocket, 0);

//		printf("%s\n", alpha[decryptSum]);

	//	}

	}

	//for(int x = 0; x < buffer[x]; x++){

	//printf("%s\n", buffer);

	//}


	//sends to dec_client
	charWritten = send(connectionSocket, buffer, readSocket, 0);
	if(charWritten < 0){

		error("SERVER: ERROR writing socket");

	}

	close(connectionSocket);

}

int main(int argc, char *argv[]){
  	int connectionSocket, charsRead;
	int charsWritten;
	int next;
	int pInt;
	char finalKey[SIZE];
  	char buffer[SIZE];
	char clientAccepted[] = "accept";
	char clientRejected[] = "reject";
	char ciphText[SIZE];
	pid_t childPid;
  	struct sockaddr_in serverAddress, clientAddress;
  	socklen_t sizeOfClientInfo = sizeof(clientAddress);

  // Check usage & args
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    exit(1);
  } 
  
  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    error("ERROR opening socket");
  }

  // Set up the address struct for the server socket
  setupAddressStruct(&serverAddress, atoi(argv[1]));

  // Associate the socket to the port
  if (bind(listenSocket, 
          (struct sockaddr *)&serverAddress, 
          sizeof(serverAddress)) < 0){
    error("ERROR on binding");
  }

  // Start listening for connetions. Allow up to 5 connections to queue up
  listen(listenSocket, 5); 
  
  // Accept a connection, blocking if one is not available until one connects
	pInt = 0;

  	while(1){
    // Accept the connection request which creates a connection socket
    	connectionSocket = accept(listenSocket, 
                	(struct sockaddr *)&clientAddress, 
                	&sizeOfClientInfo); 
    	if (connectionSocket < 0){
      		error("ERROR on accept");
    	}

		if(pInt < 5){

			childPid = fork();

			if(childPid == -1){
	
				error("ERROR on fork");

			}
			//decryption process
			else if(childPid == 0){

				close(listenSocket);
				decryption(connectionSocket, buffer, finalKey, ciphText, clientAccepted, clientRejected);

				exit(0);

			}
			else{

				pInt++;

			}

		}

		else{

			fprintf(stderr, "Waiting for completion\n");

		}

		for(int i = 0; i < 5; i++){

			if(waitpid(-1, &next, WNOHANG) == -1){

				fprintf(stderr, "SERVER: ERROR waitpid\n");

			}
			if(WIFEXITED(next)){

				pInt--;

			}

		}

		

    /*printf("SERVER: Connected to client running at host %d port %d\n", 
                          ntohs(clientAddress.sin_addr.s_addr),
                          ntohs(clientAddress.sin_port));

    // Get the message from the client and display it
    memset(buffer, '\0', 256);
    // Read the client's message from the socket
    charsRead = recv(connectionSocket, buffer, 255, 0); 
    if (charsRead < 0){
      error("ERROR reading from socket");
    }
    printf("SERVER: I received this from the client: \"%s\"\n", buffer);

    // Send a Success message back to the client
    charsRead = send(connectionSocket, 
                    "I am the server, and I got your message", 39, 0); 
    if (charsRead < 0){
      error("ERROR writing to socket");
    }
    // Close the connection socket for this client
    close(connectionSocket); */
  }
  // Close the listening socket
  close(listenSocket); 
  return 0;
}

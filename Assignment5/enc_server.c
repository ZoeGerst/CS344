


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

void encryption(int connectionSocket, char* buffer, char* finalKey){

	int firstWord;
	int secondWord;
	int encryptSum = 0;
	char alpha[] = "ABCDEFGHIJKLMNOPQRUSTUVWXYZ ";
	int charWritten;
	

	for(int i = 0; i < strlen(buffer); i++){




		for(int j = 0; j < 27; j++){

			if(buffer[i] == alpha[j]){

				firstword = j;

			}

			if(finalKey[i] == alpha[j]){

				secondWord = j;

			}


		}
		encryptSum = (firstWord + secondWord) % 27;
		buffer[i] = alpha[encryptSum];

	}

	charWritten = send(conectionSocket, buffer, strlen(buffer), 0);

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
			else if(childPid == 0){

				close(listenSocket);
				encryption(connectionSocket, buffer, finalKey);

				exit(0);

			}
			else{

				pInt++;

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

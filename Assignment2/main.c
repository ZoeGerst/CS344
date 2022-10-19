//Zoe Gerst
//Assignment 2: Files and Directories
//10/24/2022
//CS344
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Movie data struct
struct movie{

	int *year;
	double rating;
	char *title;
	char *language;
	struct movie *next;

};

struct movie *createMovie(char *currLine){

	struct movie *currMovie = malloc(sizeof(struct movie));
	//use with strtok_r
	char *saveptr;

	//First token is title
	char *token = strtok_r(currLine, ",", &saveptr);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

	char *temp;
	//Next token is year
	token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = (int*)calloc(strlen(token) + 1, sizeof(int));
	*currMovie->year = atoi(token);
	
	//printf("%d\n", *currMovie->year);
	
	//Next token is language
	token = strtok_r(NULL, ",", &saveptr);
	currMovie->language = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->language, token);

	//Last token is rating
	token = strtok_r(NULL, "\n", &saveptr);
	char *ptr;
	char *ptr2;
	char *temp2;
	temp2 = calloc(strlen(token) + 1, sizeof(char));
	currMovie->rating = strtod(token, &ptr);

	//Next node is set to NULL for the next entry
	currMovie->next = NULL;
	return currMovie;

}

//Returns a linked list of movie data through parsing
struct movie *processFile(char *filePath){

	//opens specified file
	FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

	//head of linked list
	struct movie *head = NULL;
	//tail of linked list
	struct movie *tail = NULL;

	//Reads first line
	nread = getline(&currLine, &len, movieFile);

	while ((nread = getline(&currLine, &len, movieFile)) != -1){

		struct movie *newNode = createMovie(currLine);

		//is this the first node in the linked list?
		if(head == NULL){

			//sets head and tail to this node
			head = newNode;
            tail = newNode;

		}
		else{

			//add this node to the list and advance the tail
			tail->next = newNode;
            tail = newNode;

		}

	}
	free(currLine);
    fclose(movieFile);
    return head;

}




int main(){



}

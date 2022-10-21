//Zoe Gerst
//Assignment 2: Files and Directories
//10/24/2022
//CS344
//Works Cited:
//Canvas page: "Exploration: Directories"
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>



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

struct dirent *largest(){

	DIR* currDir = opendir(".");
	struct dirent *aDir;
	struct dirent *tempDir;
	struct dirent *largeDir;
	char *file;
	char prefix[] = "movies";
	char nextDir[256];
	struct stat temp;
	struct stat infoDir;
	
	
	if(currDir > 0){

		while((aDir = readdir(currDir)) != NULL){

			if(strstr(aDir->d_name, prefix) != NULL && strstr(aDir->d_name, ".csv") != NULL){

				stat(aDir->d_name, &infoDir);
				off_t a = infoDir.st_size;
				tempDir = aDir;

				while((tempDir = readdir(currDir)) != NULL){

					if(strstr(tempDir->d_name, prefix) != NULL && strstr(tempDir->d_name, ".csv") != NULL){

						stat(tempDir->d_name, &temp);
						off_t b = temp.st_size;

						if(a <= b){

							largeDir = tempDir;

						}

						else if(a > b){

							largeDir = aDir;

						}

					}

				}

			}

		}
		printf("Now processing the chosen file name %s\n", largeDir->d_name);
		return largeDir;
	}


	closedir(currDir);

}

struct dirent *smallest(){

	DIR* currDir = opendir(".");
	struct dirent *aDir;


	closedir(currDir);

}


int main(){



}

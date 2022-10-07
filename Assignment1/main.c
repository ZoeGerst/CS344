//Zoe Gerst
//Assignment 1: Movies
//10/10/2022
//CS344
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie{

	int *year;
	double *rating;
	char *title;
	char *language;
	struct movie *next;

};

struct movie *createMovie(char *currLine){

	struct movie *currMovie = malloc(sizeof(struct movie));
	char *saveptr;

	char *token = strtok_r(currLine, " ", &saveptr);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

	char *temp;
	token = strtok_r(NULL, " ", &saveptr);
    currMovie->year = (int*)calloc(strlen(token) + 1, sizeof(int));
	
	printf("%d", currMovie->year);

/*	token = strtok_r(NULL, " ", &saveptr);
	currMovie->language = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->language, token);

	token = strtok_r(NULL, "\n", &saveptr);
	char *ptr;
	char *ptr2;
	char *temp2;
	temp2 = calloc(strlen(token) + 1, sizeof(char));
	currMovie->rating = strtod(temp2, &ptr);

	currMovie->next = NULL;*/
	return currMovie;

}

struct movie *processFile(char *filePath){

	FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    struct movie *head = NULL;
    struct movie *tail = NULL;

	while ((nread = getline(&currLine, &len, movieFile)) != -1){

		struct movie *newNode = createMovie(currLine);

		if(head == NULL){

			head = newNode;
            tail = newNode;

		}
		else{

			tail->next = newNode;
            tail = newNode;

		}

	}
	free(currLine);
    fclose(movieFile);
    return head;

}

void printMovie(struct movie* aMovie){

	printf("%s, %s %s, %s\n", aMovie->title, aMovie->year, aMovie->language, aMovie->rating);

}

void printMovieList(struct movie *list){

	while(list != NULL){

		printMovie(list);
		list = list->next;

	}

}

int main(int argc, char *argv[]){

	if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.txt\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
//    printMovieList(list);
    return EXIT_SUCCESS;

}

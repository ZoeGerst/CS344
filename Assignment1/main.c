//Zoe Gerst
//Assignment 1: Movies
//10/10/2022
//CS344
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie{

	int year;
	double rating;
	char *title;
	char language;
	struct movie *next;

}

struct movie *createMovie(char *currLine){

	struct movie *currMovie = malloc(sizeof(struct movie));
	char *saveptr;

	char *token = strtok_r(currLine, " ", &saveptr);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

	token = strtok_r(NULL, " ", &saveptr);
    currMovie->year = atoi(token);

	token = strtok_r(NULL, " ", &saveptr);
	currMovie->language = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->language, token);

	token = strtok_r(NULL, "\n", &saveptr);
	char *ptr;
	currMovie->rating = strtod(token, &ptr);

	currMovie->next = NULL
	return currMovie;

}

struct student *processFile(char *filePath){



}

void printMovieList(struct movie *list){



}

int main(int argc, char *argv[]){



}

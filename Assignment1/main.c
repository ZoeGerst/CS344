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

	token = strtok_r(NULL, " ", &saveptr);
	currMovie->language = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->language, token);

	token = strtok_r(NULL, "\n", &saveptr);
	char *ptr;
	char *ptr2;
	char *temp2;
	temp2 = calloc(strlen(token) + 1, sizeof(char));
	currMovie->rating = strtod(token, &ptr);

	currMovie->next = NULL;
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
/*
void printMovie(struct movie* aMovie){

	printf("%s, %s %s, %s\n", aMovie->title, aMovie->year, aMovie->language, aMovie->rating);

}
*/
void printMovieList(struct movie *list, int inputYear){

	int searchMovie = 0;
	
	while(list != NULL){

		if(list->year == inputYear){
			printf("%s\n", list->title);

		}

		list = list->next;
	}
	
}

void printRatingValue(struct movie *rateValue){

	for(int i = 1900; i <= 2021; i++){

		struct movie *currRate = rateValue;
		struct movie *zeroRate = NULL;
		struct movie *maxRate = NULL;

		while(currRate != NULL){

			if(currRate->year == i){

				zeroRate = currRate;
				while(zeroRate != NULL){

					if(zeroRate->rating >= currRate->rating){

						maxRate = currRate;

					}
					else{

						maxRate = currRate;

					}
					zeroRate = zeroRate->next;

				}
				printf("%d, %.2f, %s\n", maxRate->year, maxRate->rating, maxRate->title);
				break;

			}
			else if(currRate->year != i){

				currRate = currRate->next;

			}

		}

	}

}

void printLangs(struct movie *langs){

	struct movie *currLanguage = NULL;
	char *choiceL[20];
	printf("Enter the language for which you want to see movies: ");
	scanf("%s", choiceL);
	currLanguage = langs;

	while(currLanguage != NULL){

		if(strstr(currLanguage->language, choiceL)){

			printf("%d %s\n", currLanguage->year, currLanguage->title);

		}
		currLanguage = currLanguage->next;

	}

}

int options(struct movie* movieInfo){

	int input;
	int yearNum;
	int redo = 1;

	do{

		printf("1. Show movies released in the specified year\n");
		printf("2. Show highest rated movie for each year\n");
		printf("3. Show the title and year of release of all movies in a specific language\n");
		printf("4. Exit from the program\n\n");
		printf("Enter a choice from 1 to 4: ");
		scanf("%d", &input);

		if(input == 1){

			printf("Enter the year for which you want to see movies: ");
			scanf("%d", &yearNum);
			printMovieList(movieInfo,yearNum);
			printf("\n");

		}
		else if(input == 2){

			printRatingValue(movieInfo);
			printf("\n");

		}
		else if(input == 3){

			printLangs(movieInfo);
			printf("\n");

		}
		else if(input == 4){

			redo = 0;
			break;

		}

	}
	while(redo);

}

int main(int argc, char *argv[]){

	int data;
	if (argc < 2){

        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.txt\n");
        return EXIT_FAILURE;
    }
    struct movie *movieInfo = processFile(argv[1]);
	printf("Processed file %s and parsed data for %d movies\n", argv[1], data);
	options(movieInfo);
//    printMovieList(list);
    return EXIT_SUCCESS;

}

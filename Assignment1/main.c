//Zoe Gerst
//Assignment 1: Movies
//10/10/2022
//CS344
//Works cited:
//https://replit.com/@cs344/studentsc#main.c
//https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
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
//	nread = getline(&currLine, &len, movieFile);
//	Reads file line by line
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
/*
void printMovie(struct movie* aMovie){

	printf("%s, %s %s, %s\n", aMovie->title, aMovie->year, aMovie->language, aMovie->rating);

}
*/

//print data for the year the movie came out
void printMovieList(struct movie *list, int inputYear){

	//movies selected given the year
	int searchMovie = 0;
	
	while(list != NULL){

//		printf("%d\n", *list->year);
		if(*list->year == inputYear){
			//prints the movies that came out that year
			printf("%s\n", list->title);
			searchMovie = 1;
	
		}

		list = list->next;
	}
	//if no movies came out that year
	if(searchMovie == 0){

		printf("No data about movies released in the year %d\n\n", inputYear);
	}
	
}

void printRatingValue(struct movie *rateValue){

	//starting and ending dates
	for(int i = 1900; i <= 2021; i++){ 

		//current rate
		struct movie *currRate = rateValue;
		//rate number
		struct movie *zeroRate = NULL;
		//highest rating that year
		struct movie *maxRate = NULL;

		while(currRate != NULL){

			if(*currRate->year == i){

				//giving the rating the current value
				zeroRate = currRate;
				while(zeroRate != NULL){

					//points to the current rate in the file
					if(zeroRate->rating >= currRate->rating){

						maxRate = currRate;

					}
					//assigns highest rating
					else{

						maxRate = currRate;

					}
					zeroRate = zeroRate->next;

				}
				printf("%d %.1f %s\n", *maxRate->year, maxRate->rating, maxRate->title);
				break;

			}
			else if(*currRate->year != i){

				currRate = currRate->next;

			}

		}

	}

}

//prints movies that are available in a specific language
void printLangs(struct movie *langs){

	struct movie *currLanguage = NULL;
	//maximum amount of characters the user can use
	char choiceL[20];
	//enters desired language
	printf("Enter the language for which you want to see movies: ");
	scanf("%s", choiceL);
	//current language is in the list
	currLanguage = langs;

	while(currLanguage != NULL){

		//if substring does not equal NULL list is printed
		if(strstr(currLanguage->language, choiceL) != NULL){

			printf("%d %s\n", *currLanguage->year, currLanguage->title);

		}
		//no movies that were made that year is included in the data
		else{

			printf("No data about movies released in %s\n\n", choiceL);
			break;

		}
		//goes through list
		currLanguage = currLanguage->next;

	}

}

//displays user's choices
int options(struct movie* movieInfo){

	int input;
	int yearNum;
	int redo = 1;//repeats the program until specified otherwise

	do{

		printf("1. Show movies released in the specified year\n");
		printf("2. Show highest rated movie for each year\n");
		printf("3. Show the title and year of release of all movies in a specific language\n");
		printf("4. Exit from the program\n\n");
		printf("Enter a choice from 1 to 4: ");
		//user's input for the options
		scanf("%d", &input);

		//user enters the year they want to see which movies came out
		if(input == 1){

			printf("Enter the year for which you want to see movies: ");
			scanf("%d", &yearNum);
			printMovieList(movieInfo,yearNum);
			printf("\n");

		}
		//takes program to the rating function
		else if(input == 2){

			printRatingValue(movieInfo);
			printf("\n");

		}
		//user chooses a language then the language function list what's in the data
		else if(input == 3){

			printLangs(movieInfo);
			printf("\n");

		}
		//exits program
		else if(input == 4){

			redo = 0;
			break;

		}
		//error message
		else{

			printf("You entered an incorrect choice. Try again.\n\n");

		}

	}
	while(redo);//keep program in a loop

}


//processes the file
int main(int argc, char *argv[]){

	int data;
	if (argc < 2){

        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.txt\n");
        return EXIT_FAILURE;
    }
    struct movie *movieData = processFile(argv[1]);
	printf("Processed file %s and parsed data for %d movies\n\n", argv[1], data);
	options(movieData);
//    printMovieList(list);
    return EXIT_SUCCESS;

}

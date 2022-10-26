//Zoe Gerst
//Assignment 2: Files and Directories
//10/24/2022
//CS344
//Works Cited:
//Canvas page: "Exploration: Directories"
//https://stackoverflow.com/questions/308695/how-do-i-concatenate-const-literal-strings-in-c
//https://stackoverflow.com/questions/3554120/open-directory-using-c
//https://stackoverflow.com/questions/61050429/cant-use-sscanf-in-c-for-char-array


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
	//printf("test creat\n");
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


char* concat(char* str1, char str2[20]){

	char *result;
	//ties two strings together
	result = (char*) malloc(strlen(str1) + strlen(str2) + 1);
	//copies string
	strcpy(result, str1);
	strcat(result, str2);
	return result;
}

//finds the largest .csv file
struct dirent *largest(){

	//starting directory
	DIR* currDir = opendir(".");
	struct dirent *aDir;
	struct dirent *tempDir;
	struct dirent *largeDir;
	char *file;
	//looks for files beginning with these chars
	char prefix[20] = "movies_";
	char nextDir[256];
	struct stat temp;
	struct stat infoDir;
	
	
	if(currDir > 0){

		//checks to make sure there is something in the directory
		//aDir = readdir(currDir)
		while((aDir = readdir(currDir)) != NULL){

			if(strstr(aDir->d_name, prefix) != NULL && strstr(aDir->d_name, ".csv") != NULL){

				//gets info of the contents
				stat(aDir->d_name, &infoDir);
				off_t a = infoDir.st_size;
				tempDir = aDir;

				//printf("hello world\n");
	//			while(1){
				while((tempDir = readdir(currDir)) != NULL){
	
				//while(readdir(currDir) != NULL){
		//		printf("testing while\n");
					//tempDir = readdir(currDir);

		//			printf(tempDir->d_name);

					if(strstr(tempDir->d_name, prefix) != NULL && strstr(tempDir->d_name, ".csv") != NULL){

						stat(tempDir->d_name, &temp);
						off_t b = temp.st_size;

						//compares files
						if(a <= b){
							largeDir = tempDir;
		//					printf("Now processing the chosen file name %s\n", largeDir->d_name);
		//					 printf("test print\n");
						}

						else if(a > b){

							largeDir = aDir;

		//					printf("Now processing the chosen file name %s\n", *largeDir->d_name);
						}

					}

				}

			}

		}
		printf("Now processing the chosen file name %s\n", largeDir->d_name);
	}


	closedir(currDir);
	return largeDir;

}

struct dirent *smallest(){

	DIR* currDir = opendir(".");
	struct dirent *aDir;
	struct dirent *tempDir;
	struct dirent *smallDir;
	char *file;
	char prefix[20] = "movies_";
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

							smallDir = aDir;

						}

						else if(a > b){

							smallDir = tempDir;

						}

					}

				}

			}

		}
		printf("Now processing the chosen file name %s\n", smallDir->d_name);
	}

	closedir(currDir);
	return smallDir;

}

struct dirent *check(char *file){

	DIR* currDir = opendir(".");
	struct dirent *aDir;
	int fileExist = 0;

	if(currDir > 0){

		while((aDir = readdir(currDir)) != NULL){

			if(strcmp(aDir->d_name, file) == 0){

				fileExist = 1;
				return aDir;

			}

		}
		if(fileExist == 0){

			printf("File %s was not found. Try again\n", file);
			return aDir = NULL;

		}

	}
	closedir(currDir);
}

char *newDir(){

	srandom(time(NULL));
	int mkdirVal;
	char num[5];

	for(int i = 0; i < 5; i++){

		num[i] = 0;

	}
	char *name = concat("gerstz.movies.", num);
	long int ranNum = random() % 99999;
	sprintf(num, "%d", ranNum);
	strcat(name, num);
	struct stat st = {0};

	mkdirVal = mkdir(name, 0750);

	if(mkdirVal == 0){

		printf("Created directory with name %s \n", name);
		return name;

	}
	else{

		printf("Cannot create Directory\n");
		exit(1);

	}
	

}

void newFile(char *file){

	printf("Now processing the chosen file named %s\n", file);
	char *nDir = newDir();
	char num[20];
	DIR* currDir = opendir(nDir);
	struct movie *movFile = processFile(file);
	//movFile = processFile(file);
//	printf("test new file\n");
	FILE* point;

	if(currDir > 0){

		for(int i = 1900; i < 2021; i++){

			struct movie* temp = movFile;
			int tempVar = *temp->year;//(intptr_t)temp->year;
			
			while(temp != NULL){
		//		printf("tempVar %d\n\n", tempVar);
		//		printf("i %d\n\n", i);
				if(tempVar == i){

		//printf("test if\n");
					sprintf(num, "%d", i);
					char *first = concat(nDir, "/");
					char *second = concat(num, ".txt");
					char* strName = concat(first, second);
					point = fopen(strName, "a");

					if(point == NULL){

						printf("Unable to create file %s\n", strName);
						exit(1);

					}

					if(*temp->year == i){
						
						fprintf(point, "%s", temp->title);
					}
					fclose(point);

				}
				temp = temp->next;

			}

		}

	}
	closedir(currDir);
	struct movie* freeMovie = NULL;
	
	while(movFile != NULL){

		freeMovie->next;
		free(movFile->title);
		free(movFile->language);
		movFile = freeMovie;

	}
	free(movFile);
}

int main(){

	int redo = 1;
	int choice;
	int pickFile;

	do{

		printf("1. Select file to process\n2. Exit the program\n\n");
		printf("Enter a choice 1 or 2: ");
		scanf("%d", &choice);
		int doesExist = 1;

		if(choice == 1){

			while(doesExist == 1){

				struct dirent *large;
				struct dirent *small;
				struct dirent *specific;
				char specifyFile[20];

				printf("Which file you want to process?\n");
				printf("Enter 1 to pick the largest file\n");
				printf("Enter 2 to pick the smallest file\n");
				printf("Enter 3 to specify the name of a file\n\n");
				printf("Enter a choice from 1 to 3: ");
				scanf("%d", &pickFile);

				if(pickFile == 1){
			
			
					large = largest();
					printf("large worked\n");
					newFile(large->d_name);
					printf("new file worked\n");
					doesExist = 0;
					printf("does exist worked\n");

				}
				else if(pickFile == 2){

					small = smallest();
					newFile(small->d_name);
					doesExist = 0;

				}
				else if(pickFile == 3){

					printf("Enter the complete file name: ");
					scanf("%20s", specifyFile);
					specific = check(specifyFile);
		
					if(specific == NULL){

						doesExist = 1;

					}
					else if(specific != NULL){

						newFile(specific->d_name);
						doesExist = 0;

					}

				}
				else{

					printf("You entered an incorrect choice. Try again.\n\n");

				}
			}
		}
		else if(choice == 2){

			return 0;

		}
		else{

			printf("You entered an incorrect choice. Try again.\n\n");

		}
	}
	while(redo);
	
	return 0;

}

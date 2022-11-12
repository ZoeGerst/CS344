//Zoe Gerst
//Assignment 3: smallsh
//11/9/2022
//CS344
//Works Cited:
//https://brennan.io/2015/01/16/write-a-shell-in-c/
// https://www.geeksforgeeks.org/introduction-linux-shell-shell-scripting/
// https://www.geeksforgeeks.org/fgets-gets-c-language/
// https://stackoverflow.com/questions/47317781/using-strtok-to-store-numeric-values-in-a-2d-array
// https://www.tutorialspoint.com/cprogramming/switch_statement_in_c.htm


#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAX 2048
#define MAX_CHARS 512
int foreground = 1;

//exit status
void fore_pro(int position){

	//if status exits it
	if(WIFEXITED(position)){

		printf("exit value %d\n", WEXITSTATUS(position));

	}
	//terminted signal
	if(WIFSIGNALED(position)){

		printf("terminated by signal %d\n", WTERMSIG(position));
	}
}
/*
//get the user input
int split_line(char* useCmd[], int* bg, char cmdIn[], char cmdOut[], int userP){

	char len[MAX];

	printf(": ");
	fflush(stdout);
	fgets(len, MAX, stdin);
	//keeps blank
	len[strlen(len) - 1] = '\0';

	int new = 0;
	int i;
	int check_in = 0;
	int check_out = 0;

	//makes sure there's no more blanks
	for(i = 0; i < MAX; i++){

		if(len[i] == '\n'){

			len[i] == '\0';
			new = 1;

		}
	}
	if(!strcmp(len, "")){

		useCmd[0] = strdup("");
		return 0;

	}

	
	const char spread[2] = " ";
	char *tokens = strtok(len, spread);

	//reads each string
	for(i = 0; tokens; i++){
		if(strcmp(tokens, "&") == 0){

			*bg = 1;

		}
	
		//checks for input file
		else if(check_in == 1){

			cmdOut = strdup(tokens);
			check_in = 0;
//			printf("test cmdin %s\n", cmdIn);

		}

		//checks for input file
		else if(check_out == 1){

		//	tokens = strtok(NULL, spread);
			cmdOut = strdup(tokens);
		//	strcpy(cmdOut, tokens);
			check_out = 0;
			//printf("test cmdout %s\n", cmdOut);

		}

		//checks for > in user input
		else if(strcmp(tokens, ">") == 0){

		//	tokens = strtok(NULL, spread);
		//	cmdOut = strdup(tokens);
		//	strcpy(cmdOut, tokens);
			check_out = 1;

		}

		//checks for < in user input
		else if(strcmp(tokens, "<") == 0){

		//	tokens = strtok(NULL, spread);
		//	strcpy(cmdIn, tokens);
			check_in = 1;

		}
		else{

			useCmd[i] = strdup(tokens);

			//checks for $ at the end of the user's input
			for(int j = 0; useCmd[i][j]; j++){

				if(useCmd[i][j] == '$'){

					//replaces with pid
					useCmd[i][j] = '\0';
					snprintf(useCmd[i], 256, "%s%d", useCmd[i], userP);

				}

			}

		}
		tokens = strtok(NULL, spread);
	}
	for(int x = 0; x < 5; x++){

		printf("%s\n", useCmd[x]);

	}

	return i;
}*/

//tests other commands
void smallsh_launch(char* useCmd[], int* cStat, struct sigaction ctrlC, int* bg, char *cmdIn, char *cmdOut){

//	printf("top of function \n");
//	
//	printf("in %s\n", cmdIn);
//	 printf("out %s\n", cmdOut);		
	int userInt;
	int userOut;
	int assign;
	pid_t childPid = -5;
	childPid = fork();

	//execvp(cmdIn, useCmd);

//	printf("before switch %s\n", useCmd[0]);

	switch (childPid){

//	if(childPid == -1){
//
		case -1: 

			perror("fork");
			exit(EXIT_FAILURE);
			break;
	//	}

//	else if(childPid == 0){
	
		case 0:
		//	printf("Beginning of case 0 %s\n", useCmd[0]);

			ctrlC.sa_handler = SIG_DFL;
			sigaction(SIGINT, &ctrlC, NULL);

			//printf("test cmdIn %s\n", cmdIn);
//			printf("In %s\n", *cmdIn);
//			printf("Out %s\n", *cmdOut);
			

			//checks the input file
			if(strcmp(cmdIn, "*") != 0){

				userInt = open(cmdIn, O_RDONLY);

				if(userInt == -1){

			//	printf("test input\n");
					perror("Cannot open file for input\n");
					exit(1);

				}
				assign = dup2(userInt, 0);
				if(assign == -1){

					perror("Cannot open file for input\n");
               		exit(2);

				}
				//printf("%s\n", useCmd[0]);
				fcntl(userInt, F_SETFD, FD_CLOEXEC);

			}

			//handles output file
			if(strcmp(cmdOut, "*") != 0){

				

				userOut = open(cmdOut, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			
				if(userOut == -1){

					perror("Cannot open file for output\n");
					exit(EXIT_FAILURE);

				}
				//assigns duplicate
				assign = dup2(userOut, 1);
				if(assign == -1){

					perror("Cannot open file for output\n");
                	exit(EXIT_FAILURE);

				}
				fcntl(userOut, F_SETFD, FD_CLOEXEC);

			}

			//if file/directory doesn't exist
			if(execvp(useCmd[0], useCmd) < 0){

//				printf("in exec[0]  %s\n", useCmd[1]);
//				printf("in exec %s\n", useCmd);
				printf("%s: no such file or directory\n", useCmd[0]);
				fflush(stdout);
				exit(2);

			}
			break;
	//}
	//else{
		default:

			//background process
			if(*bg && foreground){

				pid_t nextPid;
				nextPid = waitpid(childPid, cStat, WNOHANG);
				printf("background pid is %d\n", childPid);
				fflush(stdout);

				}
			else{

				pid_t nextPid;
				nextPid = waitpid(childPid, cStat, 0);

			}

//	}
		//terminates child process
		while((childPid = waitpid(-1, cStat, WNOHANG)) > 0){

			printf("child %d terminated\n", childPid);
			fore_pro(*cStat);
			fflush(stdout);

		}
	}

}
/*
void fore_pro(int position){

	if(WIFEXITED(position)){

		printf("exit value %d\n", WEXITSTATUS(position));

	}
	if(WIFSIGNALED(position)){

		printf("terminated by signal %d", WTERMSIG(position));
	}
}
*/

//checks to see if in foreground process
void fore_pro2(int event){

	//^C^Z
	if(foreground == 1){

		char* ent_exit = "\nEntering foreground-only mode (& is now ignored)\n";
		write(STDOUT_FILENO, ent_exit, SIGRTMIN + 24);
		fflush(stdin);
		foreground = 0;

	}

	//^Z
	else{

		char* ent_exit = "\nExiting foreground-only mode\n";
		write(1, ent_exit, SIGRTMIN - 4);
		foreground = 1;

	}

}

int main(){

	int repeat = 1;
    int processes = 0;
	int exit_pro = 0;

	//char* user_str = "*";
	//char* user_res = "*";
	char* user_cmd[MAX_CHARS];

	int user_p = getpid();
	int i;

	//checks each character
	for(i = 0; i < MAX_CHARS; i++){

		user_cmd[i] = NULL;

	}	

	//handles ^C
	struct sigaction ignore_event = {0};
	ignore_event.sa_handler = SIG_IGN;
	sigfillset(&ignore_event.sa_mask);
	ignore_event.sa_flags = 0;
	sigaction(SIGINT, &ignore_event, NULL);
	

	//handles ^Z
	struct sigaction signal_event = {0};
	signal_event.sa_handler = fore_pro2;
	sigfillset(&signal_event.sa_mask);
	signal_event.sa_flags = 0;
	sigaction(SIGTSTP, &signal_event, NULL);

	do{

		char* user_str = "*";
		char* user_res = "*";
	//	char* user_cmd[MAX_CHARS];

		//Zombie Process
		int zombie_status;
		pid_t bgp;

		
		bgp = waitpid(-1, &zombie_status, WNOHANG);

		if(bgp > 0){

			//if pid is finished sending the signal			
			if(WIFEXITED(zombie_status)){

				printf("background pid %d is done: exit value %d\n", bgp, WEXITSTATUS(zombie_status));

				fflush(stdout);

			}

			//if signal is terminated
			else if(WIFSIGNALED(zombie_status)){

				printf("background pid %d is done: terminated by signal %d\n", bgp, WTERMSIG(zombie_status));
				fflush(stdout);

			}

		}


		//assign input to last
		//int last = split_line(user_cmd, &processes, user_str, user_res, user_p);

		char len[MAX];

		printf(": ");
		fflush(stdin);
		fgets(len, MAX, stdin);
		//keeps blank
		len[strlen(len) - 1] = '\0';

		int new = 0;
		int i;
		int check_in = 0;
		int check_out = 0;

		//makes sure there's no more blanks
	/*	for (i = 0; i < MAX; i++) {

			if (len[i] == '\n') {

				len[i] == '\0';
				new = 1;

			}
		}
		if (!strcmp(len, "")) {

			user_cmd[0] = strdup("");
	//		return 0;

		}
*/

		const char spread[2] = " ";
		char* tokens = strtok(len, spread);

		//reads each string
		for (i = 0; tokens; i++) {
			if (strcmp(tokens, "&") == 0) {

				processes = 1;

			}

			//checks for input file
			else if (check_in == 1) {

				user_str = strdup(tokens);
				check_in = 0;
				//			printf("test cmdin %s\n", cmdIn);

			}

			//checks for input file
			else if (check_out == 1) {

				//	tokens = strtok(NULL, spread);
				user_res = strdup(tokens);
				//	strcpy(cmdOut, tokens);
				check_out = 0;
				//printf("test cmdout %s\n", cmdOut);

			}

			//checks for > in user input
			else if (strcmp(tokens, ">") == 0) {

				//	tokens = strtok(NULL, spread);
				//	cmdOut = strdup(tokens);
				//	strcpy(cmdOut, tokens);
				check_out = 1;

			}

			//checks for < in user input
			else if (strcmp(tokens, "<") == 0) {

				//	tokens = strtok(NULL, spread);
				//	strcpy(cmdIn, tokens);
				check_in = 1;

			}
			else {

				user_cmd[i] = strdup(tokens);

				//checks for $ at the end of the user's input
				for (int j = 0; user_cmd[i][j]; j++) {

					if (user_cmd[i][j] == '$') {

						//replaces with pid
						user_cmd[i][j] = '\0';
						snprintf(user_cmd[i], 256, "%s%d", user_cmd[i], user_p);

					}

				}

			}
			tokens = strtok(NULL, spread);
		}


		//if the user doesn't enter anything
		if(strlen(len) == 0){

			repeat = 1;
		
		}

		//let's user use the # for commenting
		else if(strncmp(user_cmd[0], "#", 1) == 0){

			repeat = 1;

		}

		//ends program
		else if(strncmp(user_cmd[0], "exit", 4) == 0){

			//printf("exit\n");
			repeat = 0;
			fflush(stdout);
	
/*			for(i = 0; i < processes; i++){

				kill(list[processes], SIGTERM);

			}*/

		}

		//allows user to change directories
		else if(strncmp(user_cmd[0], "cd", 2) == 0){

			//cd + name
			if(user_cmd[1]){

				//if cd + name doesn't exist
				if(chdir(user_cmd[1]) == -1){

					printf("%s: no such file or directory\n", user_cmd[1]);
					fflush(stdout);

				}

			}
			//goes to home directory
			else{

				chdir(getenv("HOME"));

			}

		}

		//checks status
		else if(strncmp(user_cmd[0], "status", 6) == 0){

			fore_pro(exit_pro);

		}

		//other input
		else{

//			printf("%s\n", *user_cmd);
		//	printf("before other: %s\n", &user_cmd);
			
			user_cmd[i] = NULL;

//			printf("before other: %s\n", user_cmd);
	//		printf("Test user_ str %s", *user_str);
	//		printf("Test user_ res %s", *user_res);
//			printf("test before small\n");
			smallsh_launch(user_cmd, &exit_pro, signal_event, &processes, user_str, user_res);

		}

		//cleans code
		for(i = 0; user_cmd[i]; i++){

			user_cmd[i] = NULL;

		}

	//	processes = 0;
	//	user_str[0] = '\0';
	//	user_res[0] = '\0';


	}
	while(repeat);
	return 0;
}

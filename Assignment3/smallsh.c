//Zoe Gerst
//Assignment 3: smallsh
//11/9/2022
//CS344
//Works Cited:
//https://brennan.io/2015/01/16/write-a-shell-in-c/


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

struct command{

	int argc;
	int last;
	int move_in;
	int move_out;
	char *in;
	char *out;

};

char *read_line(){

	char *line;
	size_t bufsize = MAX_CHARS;
	//line = malloc(bufsize);
	fgets(line, MAX_CHARS, stdin);
	printf("test read line");
	return line;

}
/*
#define SMALLSH_TOK_DELIM " \t\r\n\a"
char **split_line(char *line, struct command *object){

	int bufsize = MAX, position = 1;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;
	object->last = 0;
	object->move_in = 0;
	object->move_out = 0;
	object->argc = 0;
//	object->in = 1;

	if(!tokens){

		fprintf(stderr, "Allocation Error\n");
    	exit(EXIT_FAILURE);

	}

	token = strtok(line, SMALLSH_TOK_DELIM);
	while(token != NULL && position != 0){

		if(strcmp(token, "&") == 0){

			object->last = 1;
			position = 0;

		}

		else if(strcmp(token, "<") == 0){

			object->move_in = 1;
			position = 2;

		}

		else if(strcmp(token, ">") == 0){

			object->move_out = 1;
			position = 3;

		}
		else{

			if(position == 2){

				object->in = token;

			}
			else if(position == 3){

				object->out = token;

			}
			else{

				tokens[object->argc] = token;
				object->argc++;
				tokens[object->argc] = NULL;

			}

		}

		token = strtok(NULL, SMALLSH_TOK_DELIM);
	}

	return tokens;
}

void smallsh_launch(char **args, struct command *object){

	int file_des_in;
	int file_des_out;
	char name[MAX] = "";

	if(object->move_in){

		file_des_in = open(object->in, O_RDONLY);

		if(file_des_in == -1){

			printf("Cannot open file for input\n", object->in);
			exit(EXIT_FAILURE);

		}

		if(dup2(file_des_in, 0) == -1){

			printf("Cannot open file for input\n", object->in);
			exit(EXIT_FAILURE);

		}
		fcntl(file_des_in, F_SETFD, FD_CLOEXEC);

	}

	else if(object->last){

		file_des_in == open("/dev/null", O_RDONLY);

		if(file_des_in == -1){

			printf("Cannot open file for input\n");
			exit(EXIT_FAILURE);

		}
		if(dup2(file_des_in, 0) == -1){

			printf("Cannot open file for input\n");
			exit(EXIT_FAILURE);

		}
		fcntl(file_des_in, F_SETFD, FD_CLOEXEC);

	}

	if(object->move_out){

		file_des_out = open(object->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if(file_des_out == -1){

			printf("Cannot open file for output\n", object->out);
			exit(EXIT_FAILURE);

		}

		if(dup2(file_des_out, 1) == -1){

			printf("Cannot open file for output\n", object->out);
			exit(EXIT_FAILURE);

		}
		fcntl(file_des_out, F_SETFD, FD_CLOEXEC);

	}

	if(execvp(args[0], args)){
		printf("%s: no such file or directory\n", args[0]);
		exit(EXIT_FAILURE);
	}
}
*/

void fore_pro(int position){

	//int position;
	//waitpid(wpid, &position, 0);

	if(WIFEXITED(position)){

		printf("exit value %d\n", WEXITSTATUS(position));

	}
	if(WIFSIGNALED(position)){

		printf("terminated by signal %d", WTERMSIG(position));
	//	printf("%s\n", term);
	//	fflush(stdout);

	}

}

void fore_pro2(int event){

	if(foreground == 1){

		char* ent_exit = "\nEntering foreground-only mode (& is now ignored)\n";
		write(STDOUT_FILENO, ent_exit, SIGRTMIN + 24);
		fflush(stdin);
		foreground = 0;

	}
	else{

		char* ent_exit = "\nExiting foreground-only mode\n";
		write(1, ent_exit, SIGRTMIN - 4);
//		fflush(stdout);
		foreground = 1;

	}

}

void control(char* useCmd[], int* bg, char cmdIn[], char cmdOut[], int userP){

	char len[MAX];

	printf(": ");
	fflush(stdout);
	fgets(len, MAX, stdin);
	//len = read_line();
	//printf("test read line");
//	getline(len, MAX, stdin);
	int new = 0;
	int i;

	for(i = 0; !new && i < MAX; i++){

		if(len[i] == '\n'){

			len[i] == '\0';
			new = 1;

		}
	//	scanf("%s", len[i]);

	}
	if(!strcmp(len, "")){

		useCmd[0] = strdup("");
		return;

	}
	const char spread[2] = " ";
	char *tokens = strtok(len, spread);

	for(i = 0; tokens; i++){
		if(strcmp(tokens, "&") == 0){

			*bg = 1;

		}

		else if(strcmp(tokens, ">") == 0){

			tokens = strtok(NULL, spread);
			strcpy(cmdOut, tokens);

		}
		else if(strcmp(tokens, "<") == 0){

			tokens = strtok(NULL, spread);
			strcpy(cmdIn, tokens);

		}
		else{

			useCmd[i] = strdup(tokens);

			for(int j = 0; useCmd[i][j]; j++){

				if(useCmd[i][j] == '$' && useCmd[i][j + 1] == '$'){

					useCmd[i][j] = '\0';
					snprintf(useCmd[i], 256, "%s%d", useCmd[i], userP);

				}

			}

		}
		tokens = strtok(NULL, spread);
	}
}

void other(char* useCmd[], int* cStat, struct sigaction ctrlC, int* bg, char cmdIn[], char cmdOut[]){

	int userInt;
	int userOut;
	int assign;
	pid_t childPid = -5;
	childPid = fork();

	if(childPid == -1){

		perror("fork");
		exit(EXIT_FAILURE);
		//break;

	}

	else if(childPid == 0){

		ctrlC.sa_handler = SIG_DFL;
		sigaction(SIGINT, &ctrlC, NULL);
		
		if(strcmp(cmdIn, "")){

			userInt = open(cmdIn, O_RDONLY);

			if(userInt == -1){

				perror("Cannot open file for input\n");
				exit(1);

			}
			assign = dup2(userInt, 0);
			if(assign == -1){

				perror("Cannot open file for input\n");
                exit(2);

			}
			fcntl(userInt, F_SETFD, FD_CLOEXEC);

		}

		if(strcmp(cmdOut, "")){

			

			userOut = open(cmdOut, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			
			if(userOut == -1){

				perror("Cannot open file for output\n");
				exit(EXIT_FAILURE);

			}
			assign = dup2(userOut, 1);
			if(assign == -1){

				perror("Cannot open file for output\n");
                exit(EXIT_FAILURE);

			}
			fcntl(userOut, F_SETFD, FD_CLOEXEC);

		}

		if(execvp(useCmd[0], useCmd) == -1){

			printf("%s: no such file or directory\n", useCmd[0]);
			fflush(stdout);
			exit(1);

		}
		//break;

	}
	else{

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

	}
	while((childPid = waitpid(-1, cStat, WNOHANG)) > 0){

		printf("child %d terminated\n", childPid);
		fore_pro(*cStat);
		fflush(stdout);

	}

}

int main(int argc, char ** argv){

	int repeat = 1;
    int processes = 0;
	int exit_pro = 0;	
	char status[MAX] = "No processes";
	char user_str[256] = "";
	char user_res[256] = "";
	char* user_cmd[MAX_CHARS];
	pid_t list[100];
	int user_p = getpid();
	int i;
	for(i = 0; i < MAX_CHARS; i++){

		user_cmd[i] = NULL;

	}	

	struct sigaction ignore_event = {0};
	ignore_event.sa_handler = SIG_IGN;
	sigfillset(&ignore_event.sa_mask);
	ignore_event.sa_flags = 0;
	sigaction(SIGINT, &ignore_event, NULL);
	


	struct sigaction signal_event = {0};
	signal_event.sa_handler = fore_pro2;
	sigfillset(&signal_event.sa_mask);
	signal_event.sa_flags = 0;
	sigaction(SIGTSTP, &signal_event, NULL);

//	signal_event.sa_handler = SIG_DFL;
//	pid_t childPid = -5;
//	childPid = fork();


	do{

		struct command *object = malloc(sizeof(struct command));
		object->last = 0;
		object->move_in = 0;
		object->move_out = 0;
		object->argc = 0;
		char *line;
		char **newargv;
		int zombie_status;
		pid_t bgp;

		
		bgp = waitpid(-1, &zombie_status, WNOHANG);

		if(bgp > 0){
			
			if(WIFEXITED(zombie_status)){

				printf("background pid %d is done: exit value %d\n", bgp, WEXITSTATUS(zombie_status));

				fflush(stdout);

			}
			else if(WIFSIGNALED(zombie_status)){

				printf("background pid %d is done: terminated by signal %d\n", bgp, WTERMSIG(zombie_status));
				fflush(stdout);

			}

		}

		control(user_cmd, &processes, user_str, user_res, user_p);
//		printf(": ");	
//		line = read_line();

//		newargv = split_line(line, object);

		if(user_cmd[0][0] == '\0'){

			repeat = 1;
		
		}
		else if(strncmp(user_cmd[0], "#", 1) == 0){

			//printf("Test #\n");
			repeat = 1;

		}
		else if(strcmp(user_cmd[0], "exit") == 0){

/*			if(object->argc > 2){

				printf("unexpected argument\n");
				fflush(stdout);

			}
			else if(newargv[1]){

				printf("unexpected argument\n");
				fflush(stdout);

			}
			else{

				printf("killing background processes\n");*/
			repeat = 0;
			fflush(stdout);
	
			for(i = 0; i < processes; i++){

				kill(list[processes], SIGTERM);

				//}

			}

		}
		else if(strcmp(user_cmd[0], "cd") == 0){

		/*	if(user_cmd > 2){

				printf("unexpected argument\n");
				fflush(stdout);

			}*/

			if(user_cmd[1]){

				if(chdir(user_cmd[1]) == -1){

					printf("%s: no such file or directory\n", user_cmd[1]);
					fflush(stdout);

				}

			}
			else{

				chdir(getenv("HOME"));

			}

		}
		else if(strcmp(user_cmd[0], "status") == 0){

	//		repeat = 1;
	//		printf("%s\n", status);
	//		fflush(stdout);
			fore_pro(exit_pro);

		}
		else{

			other(user_cmd, &exit_pro, signal_event, &processes, user_str, user_res);			
/*			if(childPid == 0){

			//	if(!object->last){
				signal_event.sa_handler = SIG_DFL;
				sigaction(SIGINT, &signal_event, NULL);

			//	}
				smallsh_launch(newargv, object);

			}
			else if(childPid == -1){

				perror("fork");
				exit(EXIT_FAILURE);

			}
			else{

				if(object->last && foreground){

					//list[processes] = childPid;
					//processes++;
					pid_t nextPid = waitpid(childPid, &exit_pro, WNOHANG);
					printf("background pid is %d\n", childPid);
					fflush(stdout);

				}
				else{

					pid_t nextPid;
					nextPid = waitpid(childPid, &exit_pro, 0);

				}

			}
			while((childPid = waitpid(-1, &exit_pro, WNOHANG)) > 0){

				printf("child %d terminated\n", childPid);
				fore_pro(exit_pro);
				fflush(stdout);

			}*/

		}
		for(i = 0; user_cmd[i]; i++){

			user_cmd[i] = NULL;

		}
		free(line);
		free(object);
		free(newargv);
		processes = 0;
		user_str[0] = '\0';
		user_res[0] = '\0';
		

	}
	while(repeat);

	return 0;
}

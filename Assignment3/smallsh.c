//Zoe Gerst
//Assignment 3: smallsh
//11/7/2022
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

struct command{

	int argc;
	int last;
	int move_in;
	int move_out;
	char *in;
	char *out;

};

char read_line(){

	char *line = NULL;
	ssize_t bufsize = MAX_CHARS;
	getline(&line, &bufsize, stdin);
	return line;

}

#define SMALLSH_TOK_DELIM " \t\r\n\a"
char **split_line(char *line, struct command *object){

	int bufsize = MAX, position = 1;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

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

	}

	execcvp(args[0], args);
	printf("%s: no such file or directory\n", args[0]);
	exit(EXIT_FAILURE);
}


void fore_pro(){



}



int main(int argc, char ** argv){

	int repeat = 1;
	int processes = 0;
	char status[MAX] = "No processes";
	pid_t list[100];
	struct sigaction signal_event;
	struct sigaction ignore_event;
	signal_event.sa_handler = SIG_DFL;
	ignore_event.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ignore_event, NULL);


	do{

		char *line;
		char **args;
		int zombie_status;
		pid_t bgp;

		struct command *object = malloc(sizeof(struct command));
		
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

		printf(": ");	
		line = read_line();
		args = split_line(line, object);

		

	}
	while(repeat);

	return 0;
}

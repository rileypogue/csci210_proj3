#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char *cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for (int i = 0; i < N; i++) {
		if(strcmp(cmd, allowed[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

void displayHelp() {
	printf("The allowed commands are:\n");
	for (int i = 0; i < N; i++) {
		printf("%d: %s\n", i + 1, allowed[i]);
	}
}

void changeDir(char **argv) {
	if (argv[1] != NULL && argv[2] != NULL) {
		printf("-rsh: cd: too many command arguments\n");
	} else if (chdir(argv[1] ? argv[1] : getenv("HOME")) != 0) {
		perror("-rsh: cd");
	}
}

int executeCommand(char **argv) {
	pid_t pid;
	int status;

	if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ) != 0) {
		perror("posix_spawnp");
		return -1;
	} 

	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
	}

	return status;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char *argv[21];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c
	int argc =  0;
	char *token = strtok(line, " ");
	while (token != NULL && argc < 20) {
		argv[argc++] = token;
		token = strtok(NULL, " ");
	}
	argv[argc] = NULL;

	if (argc == 0) {
		continue; // skipping empty lines
	}

	if (!isAllowed(argv[0])) {
		printf("NOT ALLOWED!\n");
		continue;
	}

	if (strcmp(argv[0], "exit") == 0) {
		return 0;
	} else if (strcmp(argv[0], "help") == 0) {
		displayHelp();
	} else if (strcmp(argv[0], "cd") == 0) {
		changeDir(argv);
	} else {
		executeCommand(argv);
	}
    }
    return 0;
}

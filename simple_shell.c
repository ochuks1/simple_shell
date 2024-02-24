/* Filename: simple_shell.c */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Function to display the shell prompt */
void display_prompt(void) {
    printf("$ ");
}

/* Function to read a command from the user */
char *read_command(void) {
    char *command = NULL;
    size_t bufsize = 0;

    if (getline(&command, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            printf("\n");
            exit(EXIT_SUCCESS);
        } else {
            perror("read_command");
            exit(EXIT_FAILURE);
        }
    }
    return command;
}

/* Function to execute a command */
void execute_command(char *command) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        char *args[] = {command, NULL};
        if (execve(command, args, NULL) == -1) {
            perror("Error");
            exit(EXIT_FAILURE);
        }
    } else {
        /* Parent process */
        do {
            if (waitpid(pid, &status, WUNTRACED) == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int main(void) {
    char *command;

    while (1) {
        display_prompt();
        command = read_command();
        execute_command(command);
        free(command);
    }

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
#define DELIMITERS " \t\r\n\a"

// Function to parse the command line input
char **parse_command(char *line) {
    int bufsize = MAX_ARGS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIMITERS);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

// Function to execute the command
void execute_command(char **args) {
    if (execvp(args[0], args) == -1) {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        exit(EXIT_FAILURE);
    }
}

int main() {
    char line[MAX_COMMAND_LENGTH];
    char **args;

    while (1) {
        printf(":) ");
        if (!fgets(line, MAX_COMMAND_LENGTH, stdin)) {
            break;
        }
        args = parse_command(line);
        if (args[0] != NULL) {
            execute_command(args);
        }
        free(args);
    }

    return EXIT_SUCCESS;
}

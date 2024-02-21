#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 64
#define MAX_ARG_LEN 64

/**
 * tokenize - Tokenizes the input command line
 * @input: The input command line
 * @args: Array to store the tokenized arguments
 */
void tokenize(char *input, char **args)
{
    char *token;
    int i = 0;

    token = strtok(input, " \t\n");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

/**
 * main - Entry point of the shell program
 * 
 * Return: Always returns 0
 */
int main(void)
{
    char input[100];
    char *args[MAX_ARGS];
    pid_t pid;
    int status;

    while (1)
    {
        // Prompt for input
        printf(":) ");
        fflush(stdout);

        // Read the input command line
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // Tokenize the input command line
        tokenize(input, args);

        // Fork a child process
        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process

            // Execute the command
            if (execvp(args[0], args) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // Parent process

            // Wait for the child process to finish
            waitpid(pid, &status, 0);
        }
    }

    return EXIT_SUCCESS;
}

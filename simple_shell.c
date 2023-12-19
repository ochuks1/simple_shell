#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * Display the shell prompt
 */
void display_prompt(void)
{
    printf("#cisfun$ ");
}

/**
 * Read a command from the user
 *
 * Return: User input as a string.
 */
char *read_command(void)
{
    char *command = NULL;
    size_t bufsize = 0;

    /* Read a line of user input */
    getline(&command, &bufsize, stdin);

    return command;
}

/**
 * Execute the given command with arguments
 * and consider the PATH environment variable.
 *
 * @command: User input command to be executed
 */
void execute_command(char *command)
{
    char *argv[] = {command, NULL};

    /* Execute the command using execve */
    if (fork() == 0)
    {
        /* Child process */
        if (strchr(command, '/') != NULL)
        {
            /* Absolute or relative path provided, no need to search in PATH */
            if (execve(command, argv, NULL) == -1)
            {
                perror("Error");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            /* Search for the command in PATH */
            char *path = getenv("PATH");
            char *token, *directory;

            token = strtok(path, ":");

            while (token != NULL)
            {
                directory = malloc(strlen(token) + strlen(command) + 2);
                if (directory == NULL)
                {
                    perror("Error");
                    exit(EXIT_FAILURE);
                }

                sprintf(directory, "%s/%s", token, command);

                if (execve(directory, argv, NULL) != -1)
                {
                    free(directory);
                    break;
                }

                free(directory);
                token = strtok(NULL, ":");
            }

            /* If the loop completes, the command was not found in PATH */
            perror("Error");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
        wait(NULL); /* Wait for the child process to finish */
    }
}

/**
 * Simple shell main function
 *
 * Description: Display a prompt, execute commands, and handle errors.
 *
 * Return: Always 0 on success.
 */
int main(void)
{
    char *command;

    while (1)
    {
        /* Display prompt and wait for user input */
        display_prompt();

        /* Read the user input command */
        command = read_command();

        /* Check for end of file (Ctrl+D) */
        if (command == NULL)
        {
            printf("\n");
            break;
        }

        /* Execute the command */
        execute_command(command);
    }

    return (0);
}

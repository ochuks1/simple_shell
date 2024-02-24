#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

/**
 * main - Entry point for the shell
 *
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    char **args;
    int status;

    while (1)
    {
        printf("$ ");
        getline(&line, &(size_t){0}, stdin);
        args = parse_input(line);
        status = execute_command(args);
        free(line);
        line = NULL;
        free(args);
    }
    return (EXIT_SUCCESS);
}

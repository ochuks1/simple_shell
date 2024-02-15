#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * execute_command - Execute the given command
 * @command: The command to execute
 */
void execute_command(char *command)
{
pid_t pid = fork();
if (pid == 0)
{
char **args = malloc(2 * sizeof(char *));
if (args == NULL)
{
perror("malloc failed");
exit(EXIT_FAILURE);
}
args[0] = command;
args[1] = NULL;
if (access(command, F_OK) != -1)
{
execve(command, args, environ);
}
else
{
fprintf(stderr, "%s: command not found\n", command);
exit(EXIT_FAILURE);
}
}
else if (pid < 0)
{
perror("Fork failed");
}
else
{
wait(NULL);
}
}
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
 * main - Simple shell program
 * Return: Always 0
 */
int main(void)
{
char *line = NULL;
size_t len = 0;
while (1)
{
printf(":) ");
getline(&line, &len, stdin);
line[strcspn(line, "\n")] = 0;  /* Remove newline character */
if (strcmp(line, "exit") == 0)
{
free(line);
exit(EXIT_SUCCESS);
}
execute_command(line);
}
free(line);
return (0);
}

void builtin_exit(void)
{
	printf(*Exiting shell...\n);
	exit(0); /*Successfully exit */
}

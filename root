#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_INPUT_LENGTH 1024

/**
 * custom_getline - Read a line of input from the user.
 *
 * This function reads input one line at a time, using a buffer to minimize calls
 * to the read system call.
 *
 * Returns: A dynamically allocated string containing the user's input or NULL
 * on end of file or error.
*/

int main()
char *custom_getline()
{
static char buffer[MAX_INPUT_LENGTH];
static char previous_input[MAX_INPUT_LENGTH] = "";
char *result = NULL;
char *newline_pos;
if (previous_input[0] != '\0')
{
strncpy(buffer, previous_input, sizeof(buffer));
previous_input[0] = '\0';
}
else
{
if (fgets(buffer, sizeof(buffer), stdin) == NULL)
{
return NULL;  /* Return NULL on end of file or error */
}
}
newline_pos = strchr(buffer, '\n');
if (newline_pos != NULL)
{
*newline_pos = '\0';
}
else
{
/* Buffer overflow or an incomplete line; handle as needed. */
}
result = strdup(buffer);
if (result == NULL)
{
/* Handle memory allocation error */
exit(1);
}
return result;
}
/**
 * main - Entry point of the simple shell program.
 *
 * This function initializes and controls the shell, allowing the user to enter
 * commands and executes them.
 *
 * Returns: 0 on successful completion.
*/

int main()
{
char *user_input;
while (1)
{
printf("$ "); /* Display a prompt */
user_input = custom_getline();
if (user_input == NULL)
{
break; /* Exit on end of file or error */
}
/* Handle the user input here (e.g., executing commands) */
free(user_input); /* Free the allocated memory */
}
printf("Goodbye!\n");
return (0);
}

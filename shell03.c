#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_PATH_LEN 1024

int main(void)
{
    char *cmd = "ls"; // Example command
    char *args[] = {cmd, "-l", "/tmp", NULL}; // Example command with arguments

    // Get the PATH environment variable
    char *path = getenv("PATH");
    if (path == NULL)
    {
        fprintf(stderr, "Error: PATH environment variable not found\n");
        exit(EXIT_FAILURE);
    }

    // Parse the PATH variable to get a list of directories
    char *dir;
    char *path_copy = strdup(path);
    if (path_copy == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(path_copy, ":");
    while (token != NULL)
    {
        char full_path[MAX_PATH_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);

        // Check if the full path exists
        if (access(full_path, X_OK) != -1)
        {
            // Command found, execute it
            pid_t pid = fork();

            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // Child process
                execve(full_path, args, NULL);
                // If execve returns, it must have failed
                perror("execve");
                exit(EXIT_FAILURE);
            }
            else
            {
                // Parent process
                wait(NULL);
                printf("Child exited\n");
                break; // Stop searching for the command
            }
        }

        token = strtok(NULL, ":");
    }

    free(path_copy);

    return EXIT_SUCCESS;
}

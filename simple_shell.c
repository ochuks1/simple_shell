#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

#define MAX_ARGS 64
#define PATH_MAX 4096

void execute_command(char *command, char *args[]);
void handle_path(char *command, char *full_path);
void handle_cd(char *args[]);
void handle_exit(void);
void handle_env(void);
void handle_setenv(char *args[]);
void handle_unsetenv(char *args[]);
void handle_logical_operators(char *command, int *status);

int main(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    char *path = "/bin";
    int status = 0;

    while (1)
    {
        printf(":) ");
        getline(&buffer, &bufsize, stdin);

        /*  Remove newline character */
        buffer[strlen(buffer) - 1] = '\0';

        /* Handle logical operators (&& and ||) */
        handle_logical_operators(buffer, &status);

        /* Skip empty commands */
        if (buffer[0] == '\0')
            continue;

        char *args[MAX_ARGS];
        args[0] = strtok(buffer, " ");

        int arg_count = 0;
        while (args[arg_count] != NULL && arg_count < MAX_ARGS - 1)
        {
            arg_count++;
            args[arg_count] = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        /* Handle built-in commands */
        if (strcmp(args[0], "cd") == 0)
        {
            handle_cd(args);
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            handle_exit();
        }
        else if (strcmp(args[0], "env") == 0)
        {
            handle_env();
        }
        else if (strcmp(args[0], "setenv") == 0)
        {
            handle_setenv(args);
        }
        else if (strcmp(args[0], "unsetenv") == 0)
        {
            handle_unsetenv(args);
        }
        else
        {
            /* Execute external commands */
            execute_command(args[0], args);
        }
    }

    free(buffer);
    return 0;
}

void execute_command(char *command, char *args[])
{
    if (fork() == 0)
    {
        char full_path[PATH_MAX];
        handle_path(command, full_path);

        if (execve(full_path, args, NULL) == -1)
        {
            perror(full_path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(NULL);
    }
}

void handle_path(char *command, char *full_path)
{
    snprintf(full_path, PATH_MAX, "%s/%s", "/bin", command);
}

void handle_cd(char *args[])
{
    if (args[1] == NULL)
    {
        chdir(getenv("HOME"));
    }
    else if (strcmp(args[1], "-") == 0)
    {
        char *old_dir = getenv("OLDPWD");
        if (old_dir != NULL)
            chdir(old_dir);
    }
    else
    {
        if (chdir(args[1]) != 0)
            perror("cd");
    }

    /* Update PWD environment variable */
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    setenv("PWD", cwd, 1);
}

void handle_exit(void)
{
    exit(EXIT_SUCCESS);
}

void handle_env(void)
{
    extern char **environ;
    for (char **env = environ; *env != NULL; env++)
    {
        printf("%s\n", *env);
    }
}

void handle_setenv(char *args[])
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return;
    }

    if (setenv(args[1], args[2], 1) != 0)
        perror("setenv");
}

void handle_unsetenv(char *args[])
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return;
    }

    if (unsetenv(args[1]) != 0)
        perror("unsetenv");
}

void handle_logical_operators(char *command, int *status)
{
    char *ptr;
    if ((ptr = strstr(command, "&&")) != NULL)
    {
        *ptr = '\0'; /* Replace && with null terminator */
        execute_command(command, NULL); /* Execute the first part */
        if (*status == 0)
        {
            execute_command(ptr + 2, NULL); /* Execute the second part if the first part succeeds */
        }
    }
    else if ((ptr = strstr(command, "||")) != NULL)
    {
        *ptr = '\0'; /* Replace || with null terminator */
        execute_command(command, NULL); /* Execute the first part */
        if (*status != 0)
        {
            execute_command(ptr + 2, NULL); /* Execute the second part if the first part fails */
        }
    }
    else
    {
        /* No logical operators, execute the command as is */
        execute_command(command, NULL);
    }
}

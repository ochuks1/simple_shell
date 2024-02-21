#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024

int main(void);
void run_shell(void);
void execute_command(char *command);
void execve(command, args, environ);
void builtin_exit(void);

#endif

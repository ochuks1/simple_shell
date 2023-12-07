#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *buffer;
    size_t bufsize = 32;
    int characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    while (1)
    {
        printf("$ ");
        characters = getline(&buffer, &bufsize, stdin);
        if (characters == -1)
        {
            printf("\n");
            break;
        }
        printf("%s", buffer);
    }

    free(buffer);
    return (0);
}

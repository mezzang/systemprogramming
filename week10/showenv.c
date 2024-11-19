#include <stdio.h>
#include <unistd.h>

extern char **environ;

void main()
{
    int i;
    for(i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);

}
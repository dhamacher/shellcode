#include <stdio.h>
#include <string.h>


void function()
{
    char array[30];

    gets(array);
    printf("%s\n", array);
}


int main()
{
    function();

    return 0;
}
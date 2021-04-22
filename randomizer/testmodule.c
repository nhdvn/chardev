#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

static unsigned char receive[64];
static unsigned long long result = 0;

int main()
{
    int file = open("/dev/Number_Randomizer", O_RDWR);

    if (file < 0)
    {
        printf("Openning Device Failed. \n");

        return 1;
    }

    int length = 0;

    printf("\nHow many bytes do you want: "); 
    
    while (scanf("%d", &length))
    {
        if (1 <= length && length <= 8) break;

        printf("I can only random from 1 to 8 bytes. \n");

        printf("\nHow many bytes do you want: "); 
    }

    int status = read(file, receive, length);

    if (status < 0)
    {
        printf("Generator has failed to read a number. \n\n");

        return 1;
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            result += (unsigned long long)receive[i] << (i * 8);
        }

        printf("Your magic number is %llu \n\n", result);
    }

    return 0;
}
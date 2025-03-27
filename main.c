#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void readTemperature()
{
    FILE *file;
    char buffer[128];

    file = popen("vcgencmd measuere_temp", "r");
    if(file == NULL)
    {
        perror("Cannot execute vcgencmd");
        exit(EXIT_FAILURE);
    }

    if(fgets(buffer, sizeof(buffer), file) == NULL)
    {
        perror("Failed to read temperature");
        pclose(file);
        exit(EXIT_FAILURE);
    }
    pclose(file);

    printf("CPU temp: %s", buffer);
}

int main(int argc, char *argv[]) 
{
    if(argc != 2)
    {
        printf("Usage: %s [time in seconds]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int interval = atoi(argv[1]);
    
    if(interval <= 0)
    {
        printf("Time interval should be at least 1");
        return EXIT_FAILURE;
    }

    while(1)
    {
        readTemperature();
        sleep(interval);
    }

    return EXIT_SUCCESS;
}
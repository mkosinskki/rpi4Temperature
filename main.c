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
    printf("Hello world");
}
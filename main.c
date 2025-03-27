#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mysql/mysql.h>

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "sensorreading"

void saveTemperatureToDB(float temperature)
{
    MYSQL *connection = mysql_init(NULL);

    if(!connection)
    {
        printf("MySQL initialization failed!\n");
        return;
    }

    if(!mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0))
    {
        printf("MySQL connection failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return;
    }

    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO rpiSensorReading (temperature) VALUES ('%.2f')", temperature);
    
    if(mysql_query(connection, query))
    {
        printf("MySQL query failed: %s\n", mysql_error(connection));
    }

    mysql_close(connection);
}

void readTemperature()
{
    FILE *file;
    char buffer[128];
    float temperature;

    file = popen("sudo vcgencmd measure_temp", "r");
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

    if (sscanf(buffer, "temp=%f'C", &temperature) != 1)
    {
        fprintf(stderr, "Failed to parse temperature\n");
        exit(EXIT_FAILURE);
    }

    printf("CPU temp: %.2f\n", temperature);
    saveTemperatureToDB(temperature);
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
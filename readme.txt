Important:

If you are trying to gcc you have to use for example: gcc main.c -o main -lmysqlclient
It's important to use '-lmysqlclient'. Also to be able to use mentioned flag you have to install mysqlclient library: sudo apt install libmysqlclient-dev.

DB_NAME - yours database name
"INSERT INTO rpiSensorReading... - rpiSensorReading is table name
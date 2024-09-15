#include <stdio.h>
#include <stdlib.h>
#include "postgresql/libpq-fe.h"

void exit_nicely(PGconn * conn) {
    PQfinish(conn);
    exit(1);
}


int main(int argc, char ** argv){
    const char * conninfo;
    PGconn * conn;
    PGresult * res;
    int nFields, i, j;

    if (argc < 2) {
        printf("You need to provide connection info as a first argument\n"); 
        return 1;
    }

    conninfo = argv[1];

    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        printf("connection error: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }

    printf("success!\n"); 
    exit_nicely(conn);
    // res = PQexec(conn, "SELECT * FROM " 

    return 0; 
}

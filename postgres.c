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
    int nTuples, nFields;

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

    res = PQexec(conn, "SELECT * FROM pms5003_measurements");
    ExecStatusType execStatus = PQresultStatus(res);
    printf("ExecStatusType: %d\n", execStatus);
    if (execStatus == PGRES_COMMAND_OK) {
        printf("Command OK, got no data\n");
        exit_nicely(conn);
    } else if (execStatus == PGRES_TUPLES_OK) {
        printf("Command OK, got some pointer ass data\n");
    } else if (execStatus == PGRES_SINGLE_TUPLE) {
        printf("Command OK, got single tuple\n");
    } else {
        fprintf(stderr, "Something went wrong"); 
        exit_nicely(conn);
    } 
    nTuples = PQntuples(res); 
    nFields = PQnfields(res);
    printf("n rows: %d, n columns: %d", nTuples, nFields);

    exit_nicely(conn);
    return 0; 
}

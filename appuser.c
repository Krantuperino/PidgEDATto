#include "appuser.h"
#include <string.h>

short appuser_new(SQLCHAR* screenName, SQLCHAR* location)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    char aids[512];
    long int num;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if(!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
    }

    /* Allocate Handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /* Preparation */
    sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'", screenName);

    SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

    SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num, sizeof(num), NULL);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("User %s doesnt exist", screenName);
      return -1;
    }

    SQLCloseCursor(stmt);

    sprintf(query, "INSERT into users(screenName, usertimestamp, location) VALUES ('%s', date_trunc('second', LOCALTIMESTAMP), %s)", screeName, location);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    printf("Create user %s located in %s\n", screenName, location);
    
    return 0;
}

short appuser_remove(SQLCHAR* screenName)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;

    ret = odbc_connect(&env, &dbc);
    if(!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
    }

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    sprintf(query, "DELETE FROM users WHERE screenName='%s'", screenName);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    printf("User %s removed\n", screenName);
    return 0;
}

void main(int argc, char **argv)
{
    if(strcmp(argv[1], "new") == 0){
        appuser_new(argv[2], argv[3]);
        return;
    }
    else if(strcmp(argv[1], "remove") == 0){
        appuser_remove(argv[2]);
        return;
    }
    else{
        printf("Opcion incorrecta, las opciones son:");
        printf("\tnew\n\tremove\n");
        return;
    }
}
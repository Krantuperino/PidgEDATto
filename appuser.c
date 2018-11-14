#include "appuser.h"
#include <string.h>

char query[512];

short appuser_new(SQLCHAR* screenName, SQLCHAR* location)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;


    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if(!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
    }

    /* Allocate Handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    sprintf(query, "INSERT into users(screenname, usercreated, location) VALUES ('%s', date_trunc('second', LOCALTIMESTAMP), '%s')", screenName, location);

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

    sprintf(query, "DELETE FROM users WHERE screenname='%s'", screenName);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    printf("User %s removed\n", screenName);
    return 0;
}

int main(int argc, char **argv)
{
  if(argc == 1){
    printf("Elije una opcion\n");
    return -1;
  }
    if(strcmp(argv[1], "new") == 0){
        appuser_new((SQLCHAR*)argv[2], (SQLCHAR*)argv[3]);
        return 0;
    }
    else if(strcmp(argv[1], "remove") == 0){
        appuser_remove((SQLCHAR*)argv[2]);
        return 0;
    }
    else{
        printf("Opcion incorrecta, las opciones son:\n");
        printf("\tnew\n\tremove\n");
        return 0;
    }
}

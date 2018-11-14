#include "appfollow.h"
#include <string.h>

char * query[512];

short appfollow_new(SQLCHAR* screenName_follower, SQLCHAR* screenName_followeed)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLSMALLINT columns;
    char aids[512];
    long int num, num2, buff;
    int n=0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if(!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
    }

    /* Allocate Handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /* Preparation */
    sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'", screenName_follower);

    SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

    SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num, sizeof(num), NULL);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("User %s doesnt exist", screenName_follower);
      return -1;
    }
    printf("%ld\n", num);

    SQLCloseCursor(stmt);

    /* Preparation 2 */
    sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'", screenName_followeed);

    SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

    SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num2, sizeof(num2), NULL);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("User %s doesnt exist", screenName_followeed);
      return -1;
    }
    printf("%ld\n", num2);

    SQLCloseCursor(stmt);

    /* AND THEN ACT LIKE I KNOW NOBODY */

    sprintf(query, "INSERT INTO follows (userfollower, userfolloweed) VALUES (%ld, %ld)", num, num2);
    printf("%s\n", query);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("There was something wrong when following\n");
      return -1;
    }
    else{
      printf("User %s succesfully followed %s\n", screenName_follower, screenName_followeed);
      return 0;
    }
}

short appfollow_remove(SQLCHAR* screenName_follower, SQLCHAR* screenName_followeed)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLSMALLINT columns;
    char aids[512];
    long int num, num2, buff;
    int n=0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if(!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
    }

    /* Allocate Handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /* Preparation */
    sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'", screenName_follower);

    SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

    SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num, sizeof(num), NULL);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("User %s doesnt exist", screenName_follower);
      return -1;
    }

    SQLCloseCursor(stmt);

    /* MY MAIN GOAL IS TO BLOW UP */
    sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'", screenName_followeed);

    SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

    SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num2, sizeof(num2), NULL);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("User %s doesnt exist", screenName_followeed);
      return -1;
    }

    SQLCloseCursor(stmt);

    /* AND THEN ACT LIKE I KNOW NOBODY HAHAHAHAHAHAHAHAHAHA */

    sprintf(query, "DELETE FROM follows WHERE userfollower=%ld AND userfolloweed=%ld", num, num2);
    printf("%s\n", query);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("There was something wrong when unfollowing\n");
      return -1;
    }
    else{
      printf("User %s succesfully unfollowed %s\n", screenName_follower, screenName_followeed);
      return 0;
    }

}

void main(int argc, char **argv)
{
    if(argv[1] == NULL){
      printf("Elija una opcion\n");
      return;
    }
    if(strcmp(argv[1], "new") == 0){
        appfollow_new(argv[2], argv[3]);
        return;
    }
    else if(strcmp(argv[1], "remove") == 0){
        appfollow_remove(argv[2], argv[3]);
        return;
    }
    else{
        printf("Opcion incorrecta, las opciones son:");
        printf("\tnew\n\tremove\n");
        return;
    }
}

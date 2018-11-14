#include "appfollow.h"
#include <string.h>

char query[512];

short appfollow_new(SQLCHAR* screenName_follower, SQLCHAR* screenName_followeed)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    char aids[512];
    long int num, num2;

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

    /* Preparation 2 */
    sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'", screenName_followeed);

    SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

    SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num2, sizeof(num2), NULL);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
      printf("User %s doesnt exist", screenName_followeed);
      return -1;
    }

    SQLCloseCursor(stmt);

    /* AND THEN ACT LIKE I KNOW NOBODY */

    sprintf(query, "INSERT INTO follows (userfollower, userfolloweed) VALUES (%ld, %ld)", num, num2);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

      printf("User %s succesfully followed %s\n", screenName_follower, screenName_followeed);
      return 0;
}

short appfollow_remove(SQLCHAR* screenName_follower, SQLCHAR* screenName_followeed)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    char aids[512];
    long int num, num2;

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

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    printf("User %s succesfully unfollowed %s\n", screenName_follower, screenName_followeed);
    return 0;

}

int main(int argc, char **argv)
{
    if(argc == 1){
      printf("Elija una opcion\n");
      return -1;
    }
    if(strcmp(argv[1], "new") == 0){
        appfollow_new((SQLCHAR*)argv[2], (SQLCHAR*)argv[3]);
        return 0;
    }
    else if(strcmp(argv[1], "remove") == 0){
        appfollow_remove((SQLCHAR*)argv[2], (SQLCHAR*)argv[3]);
        return 0;
    }
    else{
        printf("Opcion incorrecta, las opciones son:");
        printf("\tnew\n\tremove\n");
        return 0;
    }
}

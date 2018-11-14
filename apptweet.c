#include "apptweet.h"
#include <string.h>

char query[512];

short apptweet_new(SQLCHAR* screenName, SQLCHAR* text)
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

    /* MA MANGO IS TO BLOW UP */

    sprintf(query, "INSERT into tweets(tweettext, tweettimestamp, userwriter) VALUES ('%s', date_trunc('second', LOCALTIMESTAMP), %ld)", text, num);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    printf("Tweet sent by %s\n", screenName);
      return 0;

}

short apptweet_remove(SQLCHAR * tweet_id)
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


    /* MA MANGO IS TO BLOW UP */

    sprintf(query, "DELETE FROM tweets WHERE tweet_id='%s'", tweet_id);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    printf("Tweet %s removed\n", tweet_id);
    return 0;

     /* BOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM */
}

short apptweet_retweet(SQLCHAR* screenName, SQLINTEGER tweet_id)
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

  /* Aqualung */
  sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'", screenName);

  SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

  SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num, sizeof(num), NULL);

  if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
    printf("User %s doesnt exist", screenName);
    return -1;
  }

  sprintf(query, "INSERT INTO tweets (retweet, userwriter) VALUES (%d, %ld)", tweet_id, num);
  SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
  printf("User %s retweeted %d", screenName, tweet_id);
  return 0;
}

int main(int argc, char **argv)
{
    char text[512];
    int i;

    if(argc==1){
      printf("Elije una opcion");
      return -1;
    }

    if(strcmp(argv[1], "new") == 0){
        for(i=3; i<=argc-1; i++){
          strcat(text, argv[i]);
          strcat(text, " ");
        }
        printf("%s\n", text);
        apptweet_new((SQLCHAR*)argv[2], (SQLCHAR*)text);
        return 0;
    }
    else if(strcmp(argv[1], "remove") == 0){
        apptweet_remove((SQLCHAR*)argv[2]);
        return 0;
    }
    else if(strcmp(argv[1], "retweet") == 0){
        apptweet_retweet((SQLCHAR*)argv[2], atol(argv[3]));
        return 0;
    }
    else{
        printf("Opcion incorrecta, las opciones son:");
        printf("\tnew\n\tremove\n\tretweet\n");
        return -1;
    }
}

#include "appreq.h"
#include <string.h>

char query[512];


short appreq_user(SQLCHAR* screenName)
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLSMALLINT columns;
    char aids[512];
    long int num, buff;
    int n=0;

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
      printf("User %s doesnt exist\n", screenName);
      return -1;
    }

    SQLCloseCursor(stmt);

    /* Query */
    sprintf(query, "SELECT screenname, user_id FROM users WHERE user_id IN (SELECT follower FROM follows WHERE followee=%ld)", num);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    printf("Seguidores: \n");

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
      ret = SQLGetData(stmt, 1, SQL_C_CHAR, aids, sizeof(aids), NULL);
      ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, buff , sizeof(num), NULL);
      printf("%s\t%ld\n", aids, num);
      n++;
    }
    printf("Numero total de seguidores: %d\n", n);
    n=0;

    SQLCloseCursor(stmt);

    sprintf(query, "SELECT screenname, user_id FROM users WHERE user_id IN (SELECT followee FROM follows WHERE follower=%ld)", num);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    printf("Seguidos: \n");

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
      ret = SQLGetData(stmt, 1, SQL_C_CHAR, aids, sizeof(aids), NULL);
      ret = SQLGetData(stmt, 2, SQL_C_SBIGINT, buff , sizeof(buff), NULL);
      printf("%s\t%ld\n", aids, num);
      n++;
    }
    printf("Numero total de seguidos: %d\n", n);

    return 0;
}

short appreq_tweets(SQLCHAR* screenName) {
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN ret;
  SQLSMALLINT columns;
  char aids[512], aidd[512];
  long int num, buff, buffb;
  int n=0;

  /* CONNECT */
  ret = odbc_connect(&env, &dbc);
  if(!SQL_SUCCEEDED(ret)) {
    return EXIT_FAILURE;
  }

  /* Allocate Handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

  /* Preparation */
  sprintf(aids, "SELECT user_id FROM users WHERE screenname='%s'\n", screenName);

  SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

  SQLBindCol(stmt, 1, SQL_C_SBIGINT, &num, sizeof(num), NULL);

  if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
    printf("User %s doesnt exist", screenName);
    return -1;
  }

  SQLCloseCursor(stmt);

  /* Query */
  sprintf(query, "SELECT tweet_id, tweettimestamp, tweettext WHERE userwriter=%ld ORDER BY tweettimestamp", num);

  SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

  printf("Tweets: ");

  while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
    ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, buff, sizeof(buff), NULL);
    ret = SQLGetData(stmt, 2, SQL_C_CHAR, aids, sizeof(aids), NULL);
    ret = SQLGetData(stmt, 3, SQL_C_CHAR, aidd, sizeof(aidd), NULL);
    printf("%ld,%s, \"%s\"\n", buff, aids, aidd);
    n++;
  }

  printf("Numero de tweets: %d", n);

  return 0;
}

short appreq_retweets(SQLINTEGER tweet_id) {
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN ret;
  SQLSMALLINT columns;
  char aids[512], aidd[512], aidf[512];
  long int num, buff, buffb;
  int n=0;

  /* CONNECT */
  ret = odbc_connect(&env, &dbc);
  if(!SQL_SUCCEEDED(ret)) {
    return EXIT_FAILURE;
  }

  /* Allocate Handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

  sprintf(query, "SELECT tweet_id, screenname, tweettimestamp, tweettext FROM tweets, users WHERE retweet=%ld", tweet_id);

  SQLExecDirect(stmt, (SQLCHAR*) aids, SQL_NTS);

  printf("Tweets que son retweets: \n");

  while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
    ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, buff, sizeof(buff), NULL);
    ret = SQLGetData(stmt, 2, SQL_C_CHAR, aids, sizeof(aids), NULL);
    ret = SQLGetData(stmt, 3, SQL_C_CHAR, aidd, sizeof(aidd), NULL);
    ret = SQLGetData(stmt, 3, SQL_C_CHAR, aidf, sizeof(aidd), NULL);
    printf("%ld,%s,%s, \"%s\"", buff, aids, aidd);
    n++;
  }

  printf("Numero de retweets: %d", n);

  return 0;
}

short appreq_maxrt() {
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN ret;
  SQLSMALLINT columns;
  char aids[512], aidd[512], aidf[512];
  long int num, buff, buffb;
  int n=0;

  /* CONNECT */
  ret = odbc_connect(&env, &dbc);
  if(!SQL_SUCCEEDED(ret)) {
    return EXIT_FAILURE;
  }

  /* Allocate Handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

  sprintf(query, "");
}

void main(int argc, char **argv)
{
    if(strcmp(argv[1], "user") == 0){
        appreq_user(argv[2]);
        return;
    }
    else if(strcmp(argv[1], "tweets") == 0){
        appreq_tweets(argv[2]);
        return;
    }
    else if(strcmp(argv[1], "retweets") == 0){
        appreq_retweets(atoi(argv[2]));
        return;
    }
    else if(strcmp(argv[1], "maxrt") == 0){
        appreq_maxrt();
        return;
    }
    else if(strcmp(argv[1], "maxfw") == 0){
        appreq_maxfw();
        return;
    }
    else{
        printf("Opcion incorrecta, las opciones son:");
        printf("\tuser\n\ttweets\n\tretweets\n\tmaxrt\n\tmaxfw\n");
        return;
    }
}

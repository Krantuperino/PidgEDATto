#include "apptweet.h"
#include <string.h>

char * query[512];

short apptweet_new(SQLCHAR* screenName, SQLCHAR* text)
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
      printf("User %s doesnt exist", screenName);
      return -1;
    }

    SQLCloseCursor(stmt);

    /* MA MANGO IS TO BLOW UP */

    sprintf(query, "INSERT into tweets(tweettext, userwriter) VALUES ('%s', %ld)", text, num);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

    if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
        printf("There seemed to be an error sending the tweet\n");
        return -1;
    }
    else{
        printf("Tweet sent by %s\n", screenName);
        return 0;
    }
}

short apptweet_remove(SQLINTEGER tweet_id)
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


    /* MA MANGO IS TO BLOW UP */

    sprintf(query, "DELETE tweets WHERE tweet_id='%ld'", tweet_id);

    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

     if(!SQL_SUCCEEDED(ret = SQLFetch(stmt))){
        printf("There seemed to be an error deleting the tweet\n");
        return -1;
    }
    else{
        printf("Tweet %ld removed\n", tweet_id);
        return 0;
    }

     /* BOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM */
}

void main(int argc, char **argv)
{
    char * text;
    int i, j, k=0, flag=0;

    if(strcmp(argv[1], "new") == 0){
        if(argv[3][0] != '"'){
            printf("Error en el imput\n");
            return;
        }
        for(i=3, j=0; argv[i]!= NULL, flag=0; i++){
            while(argv[i][k]!= '\0'){
                if(argv[i][k] == '"'){
                    flag = 1;
                }
                text[j] = argv[i][k];
                j++;
                k++;
            }
            k = 0;
        }
        apptweet_new(argv[2], text);
        return;
    }
    else if(strcmp(argv[1], "remove") == 0){
        apptweet_remove(atoi(argv[2]));
        return;
    }
    else if(strcmp(argv[1], "retweet") == 0){
        apptweet_retweet(argv[2], atoi(argv[3]));
        return;
    }
    else{
        printf("Opcion incorrecta, las opciones son:");
        printf("\tnew\n\tremove\n\tretweet\n");
        return;
    }
}
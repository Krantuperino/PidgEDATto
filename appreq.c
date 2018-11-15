#include "appreq.h"
#include <string.h>

char query[512];


short appreq_user(SQLCHAR* screenName)
{
		SQLHENV env;
		SQLHDBC dbc;
		SQLHSTMT stmt;
		SQLRETURN ret;
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
			ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, &buff , sizeof(SQLINTEGER), NULL);
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
			ret = SQLGetData(stmt, 2, SQL_C_SBIGINT, &buff , sizeof(SQLINTEGER), NULL);
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
	char aids[512], aidd[512];
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
		ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, &buff, sizeof(SQLINTEGER), NULL);
		ret = SQLGetData(stmt, 2, SQL_C_CHAR, aids, sizeof(aids), NULL);
		ret = SQLGetData(stmt, 3, SQL_C_CHAR, aidd, sizeof(aidd), NULL);
		printf("%ld,%s, \"%s\"\n", buff, aids, aidd);
		n++;
	}

	printf("Numero de tweets: %d", n);

	return 0;
}

short appreq_retweets(SQLCHAR* tweet_id) {
	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stmt;
	SQLRETURN ret;
	char aids[512], aidd[512], aidf[512];
	long int buff;
	int n=0;

	/* CONNECT */
	ret = odbc_connect(&env, &dbc);
	if(!SQL_SUCCEEDED(ret)) {
		return EXIT_FAILURE;
	}

	/* Allocate Handle */
	SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

	sprintf(query, "SELECT t1.tweet_id, t2.screenname, t1.tweettimestamp, t1.tweettext FROM(SELECT tweet_id, userwriter, tweettimestamp, tweettext FROM tweets WHERE retweet= %s ) t1JOIN(SELECT screenname, user_id FROM users) t2 ON t1.userwriter = t2.user_id", tweet_id);

	SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

	printf("Tweets que son retweets: \n");

	while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
		ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, &buff, sizeof(SQLINTEGER), NULL);
		ret = SQLGetData(stmt, 2, SQL_C_CHAR, aids, sizeof(aids), NULL);
		ret = SQLGetData(stmt, 3, SQL_C_CHAR, aidd, sizeof(aidd), NULL);
		ret = SQLGetData(stmt, 4, SQL_C_CHAR, aidf, sizeof(aidd), NULL);
		printf("%ld,%s,%s, \"%s\"", buff, aids, aidd, aidf);
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
	char aidd[512];
	long int num, buff;

	/* CONNECT */
	ret = odbc_connect(&env, &dbc);
	if(!SQL_SUCCEEDED(ret)) {
		return EXIT_FAILURE;
	}

	/* Allocate Handle */
	SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

	sprintf(query, "SELECT t2.retweet AS tweet_id, t2.count AS count, screenname  FROM users, (SELECT t1.retweet AS retweet, t1.count AS count, userwriter FROM tweets, (SELECT retweet, count(retweet) AS count FROM tweets GROUP BY retweet) AS t1 WHERE tweet_id = t1.retweet) AS t2 WHERE user_id = t2.userwriter ORDER BY count DESC LIMIT 20 ");

	SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

	printf("Los 20 tweets mas retweeteados: \n");

	while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
		ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, &buff, sizeof(SQLINTEGER), NULL);
		ret = SQLGetData(stmt, 2, SQL_C_SBIGINT, &num, sizeof(SQLINTEGER), NULL);
		ret = SQLGetData(stmt, 3, SQL_C_CHAR, aidd, sizeof(aidd), NULL);
		printf("%ld,%s,%ld", buff, aidd, num);
	}

	return 0;
}

short appreq_maxfw() {
	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stmt;
	SQLRETURN ret;
	char aidd[512];
	long int num, buff;

	/* CONNECT */
	ret = odbc_connect(&env, &dbc);
	if(!SQL_SUCCEEDED(ret)) {
		return EXIT_FAILURE;
	}

	/* Allocate Handle */
	SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

	sprintf(query, "SELECT t1.userfolloweed, t1.count, t2.screenname FROM(SELECT userfolloweed, COUNT(userfollower) AS count FROM follows GROUP BY userfolloweed) t1 JOIN (SELECT user_id, screenname FROM users) t2 ON t1.userfolloweed = t2.user_id ORDER BY t1.count DESC LIMIT 20");

	SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

	printf("Los 20 usuarios con mas seguidores: \n");

	while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
		ret = SQLGetData(stmt, 1, SQL_C_SBIGINT, &buff, sizeof(SQLINTEGER), NULL);
		ret = SQLGetData(stmt, 2, SQL_C_SBIGINT, &num, sizeof(SQLINTEGER), NULL);
		ret = SQLGetData(stmt, 3, SQL_C_CHAR, aidd, sizeof(aidd), NULL);
		printf("%ld,%s,%ld", buff, aidd, num);
	}

	return 0;
}

int main(int argc, char **argv)
{
	if(argc == 1){
		printf("Ejija una opcion.");
		return -1;	
		}
		if(strcmp(argv[1], "user") == 0){
				appreq_user((SQLCHAR*)argv[2]);
				return 0;
		}
		else if(strcmp(argv[1], "tweets") == 0){
				appreq_tweets((SQLCHAR*)argv[2]);
				return 0;
		}
		else if(strcmp(argv[1], "retweets") == 0){
				appreq_retweets((SQLCHAR*)argv[2]);
				return 0;
		}
		else if(strcmp(argv[1], "maxrt") == 0){
				appreq_maxrt();
				return 0;
		}
		else if(strcmp(argv[1], "maxfw") == 0){
				appreq_maxfw();
				return 0;
		}
		else{
				printf("Opcion incorrecta, las opciones son:");
				printf("\tuser\n\ttweets\n\tretweets\n\tmaxrt\n\tmaxfw\n");
				return -1;
		}
}

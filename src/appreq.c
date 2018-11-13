#include "../inc/appreq.h"
#include <string.h>

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

short appreq_user(SQLCHAR* screenName)
{

}

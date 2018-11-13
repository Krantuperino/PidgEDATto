#include "../inc/apptweet.h"
#include <string.h>

void main(int argc, char **argv)
{
    char * text;
    int i, j, k=1, flag=0;

    if(strcmp(argv[1], "new") == 0){
        if(argv[3][0] != '"'){
            printf("Error en el imput\n");
            return;
        }
        for(i=3, j=0; argv[i]!= NULL, flag=0; i++){
            while(argv[i][k]!= '\0'){
                if(argv[i][k] == '"'){
                    flag = 1;
                    break;
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
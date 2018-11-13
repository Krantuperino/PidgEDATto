#include "../inc/appfollow.h"
#include <string.h>

void main(int argc, char **argv)
{
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
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char * message = "111111111000000111000000";
    char * result = malloc(strlen(message) + 1);
    char * crypted = malloc(strlen(message) + 1);
    double_present(message,result,crypted);
    printf("message : %s\nresult :  %s\ncrypted : %s\n",message,result,crypted);
    return 0;
}

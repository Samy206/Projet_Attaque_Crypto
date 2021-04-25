#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    Keys * key = malloc(sizeof(Keys));
    init_key(key);
    char * message = "111111111111111111111111";
    char * crypted = malloc(25);
    present(key,message,crypted);
    printf("message : %s \ncrypted : %s\n",message,crypted);

    return 0;
}


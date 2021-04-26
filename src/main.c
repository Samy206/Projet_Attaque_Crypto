#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include "../headers/Dechiffrement/Decryption.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    Keys * key = malloc(sizeof(Keys));
    init_key(key);
    char * message = "111111111111111111111111";
    char * crypted = malloc(25 * sizeof(char));
    present(key,message,crypted);
    printf("message : %s \ncrypted : %s\n",message,crypted);
    char *decrypted = malloc(25 * sizeof(char));
    unpresent(key, crypted, decrypted);
    printf("crypted : %s \ndecrypted : %s\n",crypted,decrypted);

    return 0;
}


#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include "../headers/Dechiffrement/Decryption.h"
#include "../headers/Attaque/Attack.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    Keys * keyA = malloc(sizeof(Keys));
    generate_keys(keyA);
    Keys * keyB = malloc(sizeof(Keys));
    generate_keys(keyB);

    char message[25] = "111111111111000000000000";
    char * crypted = malloc(25 * sizeof(char));
    char * tmp = malloc(25);

    double_present(keyA,keyB,message,tmp,crypted);

    calculate_possibilities(message,crypted);


    return 0;
}


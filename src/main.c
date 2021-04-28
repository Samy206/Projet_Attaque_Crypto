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
    Keys * keyB = malloc(sizeof(Keys));
	generate_master_key(keyA);
	generate_master_key(keyB);

    char message[25] = "111111111111000000000000";
    char * crypted = malloc(25 * sizeof(char));
    char * tmp = malloc(25);

    double_present(keyA,keyB,message,tmp,crypted);

    calculate_possibilities(message,crypted);

/*    for(int i = 0; i < SIZE_ALL; i++)
    {
    printf("\n %s | %s \n",g_liste_encryption[i], g_liste_decryption[i] );
    }*/

    return 0;
}


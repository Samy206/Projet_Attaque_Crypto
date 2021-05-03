#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include "../headers/Dechiffrement/Decryption.h"
#include "../headers/Attaque/Attack.h"


int main()
{
    uint24 messageA, messageB, cryptedA, cryptedB;
    messageA.x = 0xDF2AC3;
    messageB.x = 0x4F10D3;
    cryptedA.x = 0x7313A2;
    cryptedB.x = 0x1CF0CC;
    int nb;
    calculate_possibities(messageA,cryptedA);
    nb = search_collisions();
    printf("g_cmp_couples_keys : %d\n",nb);
}
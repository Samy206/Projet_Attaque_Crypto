#ifndef ATTACK
#define ATTACK
#include "../Chiffrement/keys.h"
#include "../Chiffrement/Encryption.h"
#include "../Dechiffrement/Decryption.h"

typedef struct
{
    int indexA;
    int indexB;
}
Couple_keys;


#define SIZE 16777215

Couple_keys couples [SIZE];

uint24 g_liste_encryption[SIZE];
uint24 g_liste_decryption[SIZE];
Key keys[SIZE];

void calculate_possibities(uint24 message,uint24 crypted);
void * calculate_enc(void * message);
void * calculate_dec(void * crypted);
int search_collisions();
void * search_high(void * nothing);
void * search_low(void * nothing);

#endif
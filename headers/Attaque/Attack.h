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
void * calculate_encA(void * message);
void * calculate_decA(void * crypted);
void * calculate_encB(void * message);
void * calculate_decB(void * crypted);

int search_collisions();
void * search_highA(void * nothing);
void * search_lowB(void * nothing);
void * search_highB(void * nothing);
void * search_lowA(void * nothing);

void check_couples(uint24 message2, uint24 crypted2);

#endif

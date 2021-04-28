#ifndef _H_ATTACK
#define _H_ATTACK

#include "../Chiffrement/keys.h"
#include "../Chiffrement/Encryption.h"
#include "../Dechiffrement/Decryption.h"


typedef struct couple_strings
{
    char * strA;
    char * strB;
}
Couple_strings;

#define SIZE_ALL 3

Keys keys [SIZE_ALL];
char g_liste_encryption[SIZE_ALL][25]; //Stockage des résultats obtenus suite au chiffrement du message clair
char g_liste_decryption[SIZE_ALL][25]; //Stockage des résultats obtenus suite au déchiffrement du message crypté

void init_couple(Couple_strings * couple, char * str1 , char * str2);

void calculate_possibilities(char * message, char * crypted);
void generate_keys();
void * calcul_encryption_A(void * message);
void * calcul_encryption_B(void * message);
void * calcul_decryption_A(void * crypted);
void * calcul_decryption_B(void * crypted);

void iteration_to_key(char * entry_params, int number);
Couple_strings search_collisions();

#endif
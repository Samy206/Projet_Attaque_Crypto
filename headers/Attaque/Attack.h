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

#define SIZE_ALL 16777216

double g_liste_encryption[SIZE_ALL]; //Stockage des résultats obtenus suite au chiffrement du message clair
double g_liste_decryption[SIZE_ALL]; //Stockage des résultats obtenus suite au déchiffrement du message crypté

void init_couple(Couple_strings * couple, char * str1 , char * str2);

void calculate_possibilities(char * message, char * crypted);
void * calcul_encryption_A(void * message);
void * calcul_encryption_B(void * message);
void * calcul_encryption_C(void * message);
void * calcul_encryption_D(void * message);
void * calcul_decryption_A(void * crypted);
void * calcul_decryption_B(void * crypted);
void * calcul_decryption_C(void * crypted);
void * calcul_decryption_D(void * crypted);

void iteration_to_key(char * entry_params, int number);
Couple_strings search_collisions();

double char_to_double(char *entry);


#endif

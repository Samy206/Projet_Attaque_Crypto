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

/* Dual core */
void calculate_possibilities_dual_core(uint24 message,uint24 crypted);
void *calculate_enc_d(void *message);
void *calculate_dec_d(void *crypted);
int search_collisions_dual_core();
void *search_high_d(void *nothing);
void *search_low_d(void *nothing);

/* Quad core */
void calculate_possibilities_quad_core(uint24 message,uint24 crypted);
void *calculate_enc_q_A(void *message);
void *calculate_dec_q_A(void *crypted);
void *calculate_enc_q_B(void *message);
void *calculate_dec_q_B(void *decrypted);
int search_collisions_quad_core();
void *search_high_q_A(void *nothing);
void *search_low_q_B(void *nothing);
void *search_high_q_B(void *nothing);
void *search_low_q_A(void *nothing);

int search_collisions();

void check_couples(int * tab_index, uint24 message2, uint24 crypted2);

#endif

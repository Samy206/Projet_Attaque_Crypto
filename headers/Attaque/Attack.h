#ifndef ATTACK
#define ATTACK
#include "../Chiffrement/keys.h"
#include "../Chiffrement/Encryption.h"
#include "../Dechiffrement/Decryption.h"

/* Structure représentant la valeur de deux clés correspondant chacune à un indice dans les tableaux que nous verrons
  plus loin*/
typedef struct
{
    int indexA;
    int indexB;
}
Couple_keys;

/* Structure qui nous permet de relier le résultat d'un chiffrement à la clé qui a été utilisée */
typedef struct
{
    uint24 value;
    int index_key;
}
Element;

#define SIZE 16777215              //2^24 - 1.

Couple_keys couples[SIZE];        //Stockage des couples de clés correspondant aux collisions.

Element g_liste_encryption[SIZE]; //Stockage des valeurs obtenues suite au chiffrement d'un message clair.
Element g_liste_decryption[SIZE]; //Stockage des valeurs obtenues suite au déchiffrement d'un message chiffré.
Key keys[SIZE];                   //Stockage des clés correspondant à toutes les possibilités.



/* ----------------------------------Dual core------------------------------------ */

/*
 La fonction "calculate_possibilities_dual_core" appelle les deux fonctions threadées qui la suivent afin d'effectuer
 les calculs de chiffrement et de déchiffrement en simultané.
*/
void calculate_possibilities_dual_core(uint24 message,uint24 crypted);
void *calculate_enc_d(void *message);
void *calculate_dec_d(void *crypted);


/*
 La fonction "search_collisions_dual_core" appelle les deux fonctions threadées qui la suivent afin d'effectuer
 la recherche de collisions en simultané sur les deux moitiés du tableau "g_liste_encryption".
*/
int search_collisions_dual_core();
void *search_high_d(void *nothing);
void *search_low_d(void *nothing);

/* ----------------------------------Quad core------------------------------------ */

/*Même principe que pour le dual_core mais avec deux fois plus de fils d'exécution*/

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

/* ----------------------------------Vérification résultats------------------------------------ */

/*
 Vérification :
     Teste les couples de collisions obtenus suite à la recherche sur le premier couple clair/chiffré (m1,c1) et
     affiche ceux qui fonctionnent aussi pour (m2,c2).
*/
void check_couples(uint24 message2, uint24 crypted2);



#endif

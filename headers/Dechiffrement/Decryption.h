#ifndef DECRYPTION
#define DECRYPTION

#include "../Chiffrement/keys.h"
#include "../Chiffrement/Encryption.h"

/* Fonction réciproque de la fonction permutation. */
void un_permutation(uint24 *state);

/* Fonction réciproque de la fonction substitution. */
void un_substitution(uint24 *state);

/* Fonction réciproque de la fonction de chiffrement. */
uint24 un_present(Key * key, uint24 message);

#endif

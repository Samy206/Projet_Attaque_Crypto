#ifndef ENCRYPTION
#define ENCRYPTION

#include "keys.h"

/* Fonction de permutation,
 * agit sur un état de 24 bits. */
void permutation(uint24 *state);

/* Fonction de substitution,
 * agit sur un état de 24 bits. */
void substitution(uint24 *state);

/* Fonction de chiffrement PRESENT24.
 * appelle les fonctions de permutation,
 * de substitution. */ 
uint24 present(Key * key, uint24 message);

/* Fonction de chiffrement 2PRESENT24. */
uint24 two_present(Key * keyA, Key * keyB, uint24 message);

#endif

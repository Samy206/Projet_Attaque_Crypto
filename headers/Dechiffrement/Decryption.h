#ifndef _H_DECRYPTION
#define _H_DECRYPTION

#include "../Chiffrement/keys.h"

/* A pointer of used keys during PRESENT24. */
Keys *used_keys;

/* An Etat for decryption.
 * Double dimension in order to decrypt blocks of words. */
char **d_Etat;

/* Main decryption process function. */
void unpresent(Keys *used_keys_params, char *crypted, char *decrypted); 

void unPermutation(char *d_Etat);
void unSubstitution(char *d_Etat);
void s_box(char *entry_params);


#endif

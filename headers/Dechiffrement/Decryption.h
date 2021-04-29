#ifndef _H_DECRYPTION
#define _H_DECRYPTION
#include "../Chiffrement/keys.h"

/* Main decryption process function. */
void unpresent(Keys *used_keys_params, char *crypted, char *decrypted);
void unPermutation(char *d_Etat);
void unSubstitution(char *d_Etat);

#endif

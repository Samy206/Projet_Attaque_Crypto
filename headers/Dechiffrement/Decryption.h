#ifndef DM_CRYPTO_DECRYPTION
#define DM_CRYPTO_DECRYPTION
#include "../Chiffrement/keys.h"
#include "../Chiffrement/Encryption.h"

void un_permutation(uint24 *state);
void un_substitution(uint24 *state);

uint24 un_present(Key * key, uint24 message);


#endif

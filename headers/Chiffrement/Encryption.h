#ifndef ENCRYPTION
#define ENCRYPTION
#include "keys.h"

void permutation(uint24 *state);
void substitution(uint24 *state);

uint24 present(Key * key, uint24 message);
uint24 two_present(Key * keyA, Key * keyB, uint24 message);

#endif

#ifndef _H_ENCRYPTION
#define _H_ENCRYPTION

#include "keys.h"

char **Etat ;

void s_box(char *entry_params);
void Substitution(char *Etat);
void Permutation(char *Etat);


void present(Keys* keys,char * message, char * result);
void double_present(Keys* keysA, Keys* keysB,char * message, char * result, char * crypted);

#endif

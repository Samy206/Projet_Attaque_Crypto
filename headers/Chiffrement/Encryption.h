#include "keys.h"

#ifndef _H_ENCRYPTION
#define _H_ENCRYPTION

char **Etat ;

void s_box(char *entry_params);
void Substitution(char *Etat);
void Permutation(char *Etat);


void present(char * message, char * result);
void double_present(char * message, char * result, char * crypted);



#endif

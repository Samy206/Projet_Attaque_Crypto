#ifndef _H_DECRYPTION
#define _H_DECRYPTION
#include "../Chiffrement/keys.h"



/* An Etat for decryption.
 * Double dimension in order to decrypt blocks of words. */
char **d_Etat ;

/* Main decryption process function. */
int split_message_dec(char * message);
void unpresent(Keys *used_keys_params, char *crypted, char *decrypted);
void unPermutation(char *d_Etat);
void unSubstitution(char *d_Etat);
void s_box(char *entry_params);


#endif

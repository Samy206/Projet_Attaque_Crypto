#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"


int main()
{
    Key * keyA = malloc(sizeof(Key));
    uint24 zero;
    zero.x = 0;
    //substitution(&zero);
    //printf("new zero : %x\n",zero.x);
    //permutation(&zero);
    init_key(keyA,zero);
    uint24 res;
    res.x = present(keyA,zero).x;
    printf("new zero : %x\n",res.x);



}
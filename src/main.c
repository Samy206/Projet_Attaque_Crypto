#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"


int main()
{
    Key * keyA = malloc(sizeof(Key));
    uint24 zero;
    zero.x = 0;
    init_key(keyA,zero);
    for(int i = 0 ; i < 11 ; i++)
    {
        printf("sub key %d : %x\n",i,keyA->sub_keys[i].x);
    }
}
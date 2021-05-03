#include "../../headers/Chiffrement/Encryption.h"
#include "../../headers/Usefull_tables.h"
#include "../../headers/Chiffrement/keys.h"
#include <stdlib.h>
#include <stdio.h>

const int p_box[24] = { 0 , 6 , 12 , 18 , 1 , 7 , 13 , 19 , 2 , 8 , 14 , 20 , 3, 9 , 15 , 21 , 4 , 10 , 16 , 22 ,
                        5 ,11 , 17 , 23 };

void permutation(uint24 * state)
{
    uint24 tmp;
    tmp.x = 0;
    int i;
    int offset;

    for (i=0; i<24; i++)
    {
        offset = 23 - i;
        tmp.x |=  ( (state->x >> offset & 0x1) << (23 - p_box[i]) );
    }

    state->x &= 0x0;
    state->x |= tmp.x;
}

void substitution(uint24 * state)
{
    uint4 entry_sbox;
    uint24 tmp ;
    tmp.x = 0;
    int offset = 4;
    int i = 0;
    int shift;

    while(offset <= 24)
    {
        entry_sbox.x = 0;
        shift = 24 - offset;

        entry_sbox.x |= (state->x >> shift & 0x00000F);
        entry_sbox.x = s_box[entry_sbox.x].x;
        tmp.x <<= 4 ;
        tmp.x |= entry_sbox.x;

        i++;
        offset += 4;
    }

    state->x = tmp.x ;
}

uint24 present(Key * key, uint24 message)
{
    uint24 state;
    state.x = message.x;

    for(int i = 0 ; i < 10 ; i++)
    {
        state.x ^= key->sub_keys[i].x;
        substitution(&state);
        permutation(&state);
    }

    state.x ^= key->sub_keys[10].x;
    return state;
}

uint24 two_present(Key * keyA, Key * keyB, uint24 message)
{
    uint24 tmp;
    tmp.x = present(keyA,message).x;
    tmp.x = present(keyB,tmp).x;
    return tmp;
}

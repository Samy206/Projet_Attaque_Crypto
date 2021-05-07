#include "../../headers/Dechiffrement/Decryption.h"
#include "../../headers/Chiffrement/Encryption.h"
#include "../../headers/Chiffrement/keys.h"
#include "../../headers/Usefull_tables.h"
#include <stdio.h>
#include <stdlib.h>


const uint4 un_s_box[16] = {{5}, {14}, {15}, {8}, {12}, {1}, {2}, {13}, {11},{4},
                            {6}, {3}, {0},{7}, {9}, {10}};

const int un_p_box[24] = {  0 , 4 , 8 , 12, 16, 20, 1 , 5 , 9, 13, 17, 21, 2 , 6, 10, 14, 18, 22, 3
                            , 7, 11, 15, 19, 23
                          } ;

void un_permutation(uint24 * state)
{

    uint24 source;
    source.x = state->x;
    uint24 permutation;
    permutation.x = 0 ;
    int distance;
    int i;
    for (i=0; i<24; i++){
        distance = 23 - i;
        permutation.x |=  ((source.x >> distance & 0x1) << (23 - un_p_box[i]));
    }
    state->x = permutation.x;
}

void un_substitution(uint24 * state)
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
        entry_sbox.x = un_s_box[entry_sbox.x].x;
        tmp.x <<= 4 ;
        tmp.x |= entry_sbox.x;

        i++;
        offset += 4;
    }

    state->x = tmp.x ;
}

uint24 un_present(Key * key, uint24 message)
{
    uint24 state;
    state.x = message.x;

    state.x = state.x ^ key->sub_keys[10].x;

    for(int i = 9 ; i >= 0 ; i--)
    {
        un_permutation(&state);
        un_substitution(&state);
        state.x =state.x ^key->sub_keys[i].x;
    }

    return state;
}
#include "../../headers/Chiffrement/Encryption.h"
#include "../../headers/Usefull_tables.h"
#include "../../headers/Chiffrement/keys.h"
#include <stdlib.h>
#include <stdio.h>



void permutation(uint24 * state)
{
    int offset ;
    uint24 tmp;
    tmp.x = 0;
    int place ;
    for(int i = 0 ; i < 24 ; i++)
    {
        offset = (i*6) % 23;
        place = 23 - offset;
        tmp.x |= ( (state->x >> (place)) & (0x1) );

    }
    state->x = tmp.x;
}

void substitution(uint24 * state)
{
    uint4 entry_sbox;
    int offset = 4;
    uint24 place[6] = { {0x0FFFFF}, {0xF0FFFF}, {0xFF0FFF}, {0xFFF0FF}, {0xFFFF0F}, {0xFFFFF0} };
    int i = 0;
    int shift;

    while(offset <= 24)
    {
        shift = 24 - offset;

        entry_sbox.x = 0;
        entry_sbox.x |= state->x >> shift;
        entry_sbox.x = s_box[entry_sbox.x].x;
        state->x &= place[i].x;
        state->x |= ( (u_int64_t) (entry_sbox.x) << shift )  ;

        i++;
        offset += 4;
    }
}

uint24 present(Key * key, uint24 message)
{
    uint24 state;
    state.x = message.x;


    for(int i = 0 ; i < 10 ; i++)
    {
        printf("before i : %d, state : %x\n",i+1,state.x);
        state.x ^= key->sub_keys[i].x;
        substitution(&state);
        permutation(&state);
        printf("after i : %d, state : %x\n\n",i+1,state.x);
    }

    state.x ^= key->sub_keys[10].x;
    printf("i : %d, state : %x\n",11,state.x);
    return state;
}

uint24 two_present(Key * keyA, Key * keyB, uint24 message)
{
    uint24 tmp = present(keyA,message);
    return(present(keyB,tmp));
}
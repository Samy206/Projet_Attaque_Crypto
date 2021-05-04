#include "../../headers/Chiffrement/keys.h"
#include <stdio.h>
#include "../../headers/Usefull_tables.h"
#include <stdint.h>

const uint4 s_box[16] = { {12} , {5} , {6} , {11} , {9} , {0} ,
                          {10} , {13} , {3} , {14} , {15}, {8} , {4},
                          {7} , {1} , {2}
                        };

void init_key(Key * key, uint24 value)
{
    key->master_key.x = value.x;
    key_schedule(key);
}


void key_schedule(Key * key)
{
    uint80 k_register;
    k_register.x = key->master_key.x;
    k_register.x <<= 40;
    k_register.y = 0;

    u_int64_t k_x,k_y;

    uint4 entry_sbox;

    key->sub_keys[0].x = k_register.x;

    for(int i = 1 ; i < 11 ; i++)
    {
        k_x = k_register.x;
        k_y = k_register.y;

        k_register.x = (k_register.x << 61) | (k_y << 45) | (k_x >> 19);
        k_register.y = ((k_x >> 3) & 0xFFFF);

        entry_sbox.x = s_box[k_register.x >> 60].x;
        k_register.x &= 0x0FFFFFFFFFFFFFF;
        k_register.x |= ((uint64_t) (entry_sbox.x) << 60);
        
        k_register.y ^= ((i & 0x01) << 15) ;
        k_register.x ^= (i >> 1);

        key->sub_keys[i].x = k_register.x;
    }
}

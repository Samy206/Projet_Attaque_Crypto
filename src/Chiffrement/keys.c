#include "../../headers/Chiffrement/keys.h"
#include <stdio.h>
//#include "../../headers/Chiffrement/Encryption.h"

const uint4 s_box[16] = { {12} , {5} , {6} , {11} , {9} , {0} ,
                          {10} , {13} , {3} , {14} , {15}, {8} , {4}
                          , {7} , {1} , {2}  };

void init_key(Key * key, uint24 value)
{
    key->master_key.x = value.x;
    key_schedule(key);
}

void key_schedule(Key * key)
{
    uint64 k_register;
    uint64 k_register_tmp;
    k_register.x = key->master_key.x;
    k_register_tmp.x = key->master_key.x;
    uint4  entry_sbox;
    uint5 xored;
    for(int i = 0 ; i < 11 ; i++)
    {
        printf("k_register begin %d : %lx, %ld\n",i,k_register.x,k_register.x);
        key->sub_keys[i].x = 0;
        key->sub_keys[i].x |= (k_register.x << 23);     //Etape récup sous clé

        k_register.x >>= 61;                             //Etape décalage 61

        entry_sbox.x = 0;
        entry_sbox.x |= (k_register.x << 60);
        entry_sbox.x = s_box[entry_sbox.x].x;
        k_register.x = 0;
        k_register.x = entry_sbox.x;
        k_register.x >>=  60;
        k_register.x = k_register.x | k_register_tmp.x;                  //Etape s_box[79-76]
        printf("k_register after sbox %d : %lx\n",i,k_register.x);

        xored.x = i;

        k_register.x >>= 50;
        k_register.x ^= xored.x;
        k_register.x <<= 50;
        printf("k_register end %d : %lx\n\n",i,k_register.x);
    }
}

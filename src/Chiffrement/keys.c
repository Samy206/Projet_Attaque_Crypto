#include "../../headers/Chiffrement/keys.h"
#include <stdio.h>
#include "../../headers/Usefull_tables.h"
#include <stdint.h>
#include <math.h>

const uint4 s_box[16] = { {12} , {5} , {6} , {11} , {9} , {0} ,
                          {10} , {13} , {3} , {14} , {15}, {8} , {4},
                          {7} , {1} , {2}
                        };

void init_key(Key * key, uint24 value)
{
    key->master_key.x = value.x;
    key_schedule(key);
    key->status = 1;
}


void key_schedule(Key * key)
{
    uint80 k_register;
    k_register.high = (key->master_key.x);
    k_register.high <<= 16;
    k_register.low = 0;

    uint80 backup ;

    uint4 entry_sbox;

    key->sub_keys[0].x = (k_register.low >> 16);

    for(int i = 1 ; i < 11 ; i++)
    {

        backup.high = k_register.high;
        backup.low = k_register.low;
        /*printf("K_register before offset : ");
        print_string(backup.high);
        printf(" ");
        print_string(k_register.low);
        printf("\n");*/

        k_register.high = 0;
        k_register.high |= (k_register.low << 21);
        k_register.high |= (backup.high >> 19);

        k_register.low >>= 19;
        backup.high <<= 21;
        k_register.low |= backup.high ;
        backup.low = k_register.low;
        /*printf("K_register after offset  : ");
        print_string(k_register.high);
        printf(" ");
        print_string(k_register.low);
        printf("\n");*/

        entry_sbox.x = 0;
        entry_sbox.x |= s_box[k_register.high >> 36].x;
        k_register.high &= 0x0FFFFFFFFF;
        k_register.high |= ((uint64_t) (entry_sbox.x) << 36);
        /*printf("K_register after sbox    : ");
        print_string(k_register.high);
        printf(" ");
        print_string(k_register.low);
        printf("\n");*/

        k_register.low <<= 21;
        k_register.low >>= 36;
        k_register.low ^= i;
        k_register.low <<= 15;
        k_register.low |= (backup.low & 1099511136255);

        /*printf("K_register after xored   : ");
        print_string(k_register.high);
        printf(" ");
        print_string(k_register.low);
        printf("\n\n");*/

        key->sub_keys[i].x = (k_register.low >> 16 );
    }
}

void print_string(long int x)
{
    for(int i = 23 ; i >= 0 ; i--)
    {
        if(x >= pow(2,i))
        {
            printf("1");
            x-=pow(2,i);
        }
        else
            printf("0");
    }
}
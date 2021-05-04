#include "../../headers/Chiffrement/keys.h"
#include "../../headers/Chiffrement/Encryption.h"
#include "../../headers/Usefull_tables.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int s_box[16] = { 12 , 5 , 6 , 11 , 9 , 0 , 10 , 13 , 3 , 14 , 15 , 8  , 4 , 7 , 1 , 2 };
const int offset[80] = {61 , 62 , 63 , 64 , 65 , 66, 67 , 68 , 69 , 70 , 71 , 72 , 73 , 74 ,75 , 76 , 77 , 78 , 79 ,
                        0 , 1 , 2 ,3 , 4 , 5 , 6 , 7 ,8 , 9 , 10 , 11, 12 , 13 , 14 , 15 , 16 , 17 ,18 ,19 ,20 , 21,
                        22 , 23 ,24 ,25 , 26 ,27, 28 ,29 , 30 ,31 , 32 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 41,
                        42 , 43 ,44 , 45 , 46 , 47 , 48 , 49 , 50, 51 , 52 , 53 , 54 , 55 , 56 , 57 , 58 , 59 , 60 };

/* Generate a master key of 24 bits. */
void generate_master_key(Keys * keys)
{
    srandom(time(NULL));
    int number;
    for(int i = 0; i < 24; i++)
    {
        number = random() % 2;
        keys->g_master_key[i] = number + '0';
    }
}

void key_schedule_algorithm(Keys *keys)
{
    char K_register[81];
    char tmp[81];
    memset(K_register,0,80);
    char word_i[5];
    int i,j;
    char comparator;
    int decimal ;
    for(i = 0; i < 80 ; i++)
    {
        if(i < 24) {
            K_register[i] = keys->g_master_key[i];
            tmp[i] = keys->g_master_key[i];
        }
        else {
            K_register[i] = '0';
            tmp[i] = '0';
        }
    }
    K_register[80] = '\0';
    tmp[80] = '\0';
    for(i = 0; i < 11; i++)
    {
        memset(keys->g_sub_keys[i],0,24);
        for(j = 40; j < 64; j++)
        {
            keys->g_sub_keys[i][j-40] = K_register[j];
        }
        keys->g_sub_keys[i][24] = '\0';

	printf("K_register before offset : %s\n",K_register);
        for (j = 0; j < 80; j++) {
            K_register[j] = tmp[offset[j]];
        }
	printf("K_register  after offset : %s\n",K_register);


        for (j = 0; j < 4; j++) {
            word_i[j] = K_register[j];
        }
        word_i[4] = '\0';
        decimal = binary_to_decimal(word_i);
	printf("K_register  before s_box : %s \n",K_register);
        decimal_to_binary(s_box[decimal],word_i);

        for (j = 0; j < 4; j++) {
            K_register[j] = word_i[j];
        }

	printf("K_register   after s_box : %s \n",K_register);
        decimal_to_binary(i+1, word_i);
	printf("K_register  before xored : %s\n",K_register);
        for (j = 61; j < 65; j++) {
            if(K_register[j] == word_i[j-61])
                K_register[j] = '0';
            else
                K_register[j] = '1';
        }
	printf("K_register   after xored : %s\n\n",K_register);

        strcpy(tmp,K_register);
    }
}

void init_key(Keys *keys, char * master_key)
{
    if(strlen(master_key) != 24)
    {
        printf("strlen : %lu\n", strlen(master_key));
        return;
    }

    for(int i = 0 ; i < 24 ; i++)
    {
        keys->g_master_key[i] = master_key[i];
    }
    keys->g_master_key[24] ='\0';
    key_schedule_algorithm(keys);
}


int binary_to_decimal(char * entry_params)
{
    int size = strlen(entry_params);
    int exposant = size - 1;
    int number = 0;

    for(int i = 0 ; i < size ; i++)
    {
        if(entry_params[i] == '1')
            number += pow(2,exposant);

        exposant--;
    }

    return number;
}

void decimal_to_binary(int number, char * entry_params)
{
    int exposant = 3;
    int comparator = pow(2,exposant);
    int cmp = 0;
    while(exposant >= 0)
    {
        if(number >= comparator)
        {
            entry_params[cmp] = '1';
            number = number - comparator ;
        }
        else
        {
            entry_params[cmp] = '0';
        }
        exposant--;
        cmp++;
        comparator = pow(2,exposant);
    }
}

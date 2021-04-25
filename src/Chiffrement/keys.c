#include "../../headers/Chiffrement/keys.h"
#include "../../headers/Chiffrement/Encryption.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
    int offset ;
    char word_i[5];
    int i,j;
    char comparator;

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

        for (j = 0; j < 80; j++) {
            offset = (j + 61) % 80;
            K_register[j] = tmp[offset];
        }

        for (j = 0; j < 4; j++) {
            word_i[j] = K_register[j];
        }
        word_i[4] = '\0';
        s_box(word_i);
        for (j = 0; j < 4; j++) {
            K_register[j] = word_i[j];
        }

        decimal_to_binary(i+1, word_i);
        for (j = 61; j < 65; j++) {
            comparator = xor(K_register[j], word_i[j - 61]);
            K_register[j] = comparator;
        }

        for (j = 0; j < 80; j++) {
            tmp[j] = K_register[j];
        }
    }
}

void init_key(Keys *keys)
{
    //generate_master_key(keys);
    for(int i = 0 ; i < 24 ; i++)
    {
        keys->g_master_key[i] = '0';
    }
    keys->g_master_key[24] ='\0';
    key_schedule_algorithm(keys);
};


int split_message(char * message)
{

    int taille = strlen(message);
    int nb_blocs;

    if( (taille % 24) == 0)
        nb_blocs =  (taille / 24) ;
    else
        nb_blocs = (taille / 24) + 1;

    int i;
    Etat = malloc(nb_blocs * sizeof(char*));

    for(i = 0 ; i < nb_blocs ; i++)
    {
        Etat[i] = malloc(25 * sizeof(char));
        memset(Etat[i],0,25);
    }

    int cmp = 0;
    int j = 0;
    for(i = 0 ; i < taille ; i++)
    {
        if( i < (taille - 1) )
        {
            if (i % 24 == 0 && cmp == 0 && j != 24)
            {
                Etat[cmp][j] = message[i];
                j++;
            }
            else if (i % 24 == 0 && j == 24)
            {
                Etat[cmp][j] = '\0';

                j = 0;
                cmp++;
                Etat[cmp][j] = message[i];
                j++;

            }
            else
            {
                Etat[cmp][j] = message[i];
                j++;
            }
        }
        else {

            if (i % 24 == 23) {
                Etat[cmp][j] = message[i];
                j++;
                Etat[cmp][j] = '\0';
            } else {
                for (int k = j; k < 24; k++) {
                    Etat[cmp][j] = '0';
                    j++;
                }
                Etat[cmp][j] = '\0';
            }
        }
    }

    return nb_blocs;
}

char xor(char A, char B)
{
    if(A == B)
        return '0';

    else
        return '1';

};

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






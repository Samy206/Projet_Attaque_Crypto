#include "../../headers/Chiffrement/Encryption.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


void s_box(char *entry_params)
{
    int number = binary_to_decimal(entry_params);
    switch(number) {
        case 0:
            decimal_to_binary(12, entry_params);
            break;
        case 1:
            decimal_to_binary(5, entry_params);
            break;
        case 2:
            decimal_to_binary(6, entry_params);
            break;
        case 3:
            decimal_to_binary(11, entry_params);
            break;
        case 4:
            decimal_to_binary(9, entry_params);
            break;
        case 5:
            decimal_to_binary(0, entry_params);
            break;
        case 6:
            decimal_to_binary(10, entry_params);
            break;
        case 7:
            decimal_to_binary(13, entry_params);
            break;
        case 8:
            decimal_to_binary(3, entry_params);
            break;
        case 9:
            decimal_to_binary(14, entry_params);
            break;
        case 10:
            decimal_to_binary(15, entry_params);
            break;
        case 11:
            decimal_to_binary(8, entry_params);
            break;
        case 12:
            decimal_to_binary(4, entry_params);
            break;
        case 13:
            decimal_to_binary(7, entry_params);
            break;
        case 14:
            decimal_to_binary(1, entry_params);
            break;
        case 15:
            decimal_to_binary(2, entry_params);
            break;
    }
}
void Substitution(char *Etat)
{
    char word_i[5];
    int j = 0;
    int k ;
    for(int i = 0; i < 24; i++)
    {

        word_i[j] = Etat[i];

        j++;
        if(j == 4)
        {
            word_i[j] ='\0';
            s_box(word_i);

            for(k = i - 3 ; k <= i ; k++)
                Etat[k] = word_i[k%4];

            j = 0;
        }
    }
}


void Permutation(char *Etat)
{
    int i ;
    int cpt = 0;
    char c;

    for(i = 1; i < 23; i++)
    {
        cpt += 6;
        c = Etat[i];
        Etat[i] = Etat[cpt % 23];
        Etat[cpt % 23] = c;
    }
}


void present(char * message, char * result)
{

    Keys * keys = malloc(sizeof(Keys));
    generate_master_key(keys);
    key_schedule_algorithm(keys);
    char comparator;
    int nb_blocs = split_message(message);

    for(int i = 0 ; i < nb_blocs ; i++)
    {
        for(int j = 0 ; j < 10 ; j++)
        {

            for(int k = 0 ; k < 24 ; k++)
            {
                comparator = xor(Etat[i][k],keys->g_sub_keys[j][k]);
                Etat[i][k] = comparator;
            }
            Substitution(Etat[i]);
            Permutation(Etat[i]);
        }

        for(int l = 0 ; l < 24 ; l++)
        {
            comparator = xor(Etat[i][l],keys->g_sub_keys[10][l]);
            Etat[i][l] = comparator;
        }

    }

    int j = 0;
    for(int i = 0 ; i < nb_blocs; i++ )
    {
        for(int l = 0 ; l < 24 ; l++) {
            result[j] = Etat[i][l];
            j++;
        }
    }
    result[j] = '\0';
}

void double_present(char * message, char * result, char * crypted)
{
    present(message,result);
    present(result,crypted);
}
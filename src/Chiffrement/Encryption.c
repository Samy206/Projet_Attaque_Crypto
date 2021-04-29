#include "../../headers/Chiffrement/Encryption.h"
#include "../../headers/Chiffrement/keys.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../../headers/Usefull_tables.h"

int s_box[16] = { 12 , 5 , 6 , 11 , 9 , 0 , 10 , 13 , 3 , 14 , 15 , 8  , 4 , 7 , 1 , 2 };
int p_box[22] = {6 , 12 , 18 , 1 , 7 , 13 , 19 , 2 , 8 , 14 , 20 , 3 , 9 , 15 , 21 , 4 , 10 , 16 , 22 , 5 , 11 , 17};

void Substitution(char *Etat)
{
    char word_i[5];
    int j = 0;
    int k ;
    int decimal;
    for(int i = 0; i < 24; i++)
    {
        word_i[j] = Etat[i];
        j++;
        if(j == 4)
        {
            word_i[j] ='\0';
            decimal = binary_to_decimal(word_i);
            decimal_to_binary(s_box[decimal],word_i);
            for(k = i - 3 ; k <= i ; k++)
            {
                Etat[k] = word_i[k%4];
            }
            j = 0;
        }
    }
}

void Permutation(char *Etat)
{
    char tmp[25];

    strcpy(tmp,Etat);
    tmp[24] = '\0';

    for(int i = 1; i < 23; i++)
        Etat[p_box[i-1]] = tmp[i];

}

void present(Keys * keys, char * message, char * result)
{
    char Etat[25] ;
    strcpy(Etat,message);
    Etat[24] = '\0';
    int i, j;

    for(i = 0 ; i < 10 ; i++)
    {
        for(j = 0 ; j < 24 ; j++)
        {
            if(Etat[j] == keys->g_sub_keys[i][j])
                Etat[j] = '0';
            else
                Etat[j] = '1';
        }
        Substitution(Etat);
        Permutation(Etat);
    }
    for(i = 0 ; i < 24 ; i++)
    {
        if(Etat[i] == keys->g_sub_keys[10][i])
            Etat[i] = '0';
        else
            Etat[i] = '1';
    }

    strcpy(result,Etat);
    result[24] = '\0';
}

void double_present(Keys* keysA, Keys* keysB,char * message, char * result, char * crypted)
{
    present(keysA,message,result);
    present(keysB,result,crypted);
}

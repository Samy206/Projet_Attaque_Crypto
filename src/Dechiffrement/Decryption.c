#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "../../headers/Dechiffrement/Decryption.h"
#include "../../headers/Chiffrement/Encryption.h"
#include "../../headers/Usefull_tables.h"

int uns_box[16] = { 5, 14, 15, 8, 12, 1, 2, 13, 11, 4, 6, 3, 0, 7, 9, 10 };
void unPermutation(char *d_Etat)
{
	char tmp[25];
	strcpy(tmp,d_Etat);
	tmp[24] = '\0';

	for(int i = 1; i < 23; i++)
	    d_Etat[i] = tmp[p_box[i-1]];

}

void unSubstitution(char *d_Etat)
{
	char word_i[5];
	int j = 0;
	int k;
    int decimal;
	for(int i = 0; i < 24; i++)
	{
	    word_i[j] = d_Etat[i];
	    j++;

	    if(j == 4)
	    {
		    word_i[j] ='\0';
            decimal = binary_to_decimal(word_i);
            decimal_to_binary(uns_box[decimal],word_i);
		    for(k = i - 3 ; k <= i ; k++)
		    {
		        d_Etat[k] = word_i[k%4];
		    }
		    j = 0;
	    }
	}
}

void unpresent(Keys *used_keys_params, char *crypted, char *decrypted)
{

    int i,j;
    char d_Etat[25] ;
    strcpy(d_Etat,crypted);
    d_Etat[24] = '\0';

    for(i = 0; i < 24; i++)
    {
        if(d_Etat[i] == used_keys_params->g_sub_keys[10][i])
            d_Etat[i] = '0';
        else
            d_Etat[i] = '1';
    }
/* unpresent, xor with Ki keys must be done in reverse, K10 to K1. */

    for(i = 9; i >= 0; i--)
	{
		unPermutation(d_Etat);
		unSubstitution(d_Etat);

		for(j = 0; j < 24; j++)
		{
            if(d_Etat[j] == used_keys_params->g_sub_keys[i][j])
                d_Etat[j] = '0';
            else
                d_Etat[j] = '1';
		}
	}

	strcpy(decrypted,d_Etat);
	decrypted[24] = '\0';
	
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../headers/Dechiffrement/Decryption.h"

void unPermutation(char *d_Etat)
{
	int i;
	int cpt = 6;

	char tmp[25];
	for(int i = 0 ; i < 24 ; i++)
	{
	    tmp[i] = d_Etat[i];
	}
	tmp[24] = '\0';

	for(i = 1; i < 23; i++)
	{
	    d_Etat[i] = tmp[cpt%23];
	    cpt += 6;
	}
}

void uns_box(char *entry_params)
{
    int number = binary_to_decimal(entry_params);
    switch(number) {
        case 0:
            decimal_to_binary(5, entry_params);
            break;
        case 1:
            decimal_to_binary(14, entry_params);
            break;
        case 2:
            decimal_to_binary(15, entry_params);
            break;
        case 3:
            decimal_to_binary(8, entry_params);
            break;
        case 4:
            decimal_to_binary(12, entry_params);
            break;
        case 5:
            decimal_to_binary(1, entry_params);
            break;
        case 6:
            decimal_to_binary(2, entry_params);
            break;
        case 7:
            decimal_to_binary(13, entry_params);
            break;
        case 8:
            decimal_to_binary(11, entry_params);
            break;
        case 9:
            decimal_to_binary(4, entry_params);
            break;
        case 10:
            decimal_to_binary(6, entry_params);
            break;
        case 11:
            decimal_to_binary(3, entry_params);
            break;
        case 12:
            decimal_to_binary(0, entry_params);
            break;
        case 13:
            decimal_to_binary(7, entry_params);
            break;
        case 14:
            decimal_to_binary(9, entry_params);
            break;
        case 15:
            decimal_to_binary(10, entry_params);
            break;
    }
}

void unSubstitution(char *d_Etat)
{
	char word_i[5];
	int j = 0;
	int k;

	for(int i = 0; i < 24; i++)
	{
	    word_i[j] = d_Etat[i];
	    j++;

	    if(j == 4)
	    {
		word_i[j] ='\0';
		uns_box(word_i);
		for(k = i - 3 ; k <= i ; k++)
		{
		    d_Etat[k] = word_i[k%4];
		}
		j = 0;
	    }
	}
}

void unpresent(Keys *used_keys_params, char **used_state_params, char *crypted, char *decrypted)
{
	printf("\n\n \t Decryption process started \n");

	used_keys = used_keys_params;
	
	int str_crypted_size = strlen(crypted);
	int blocks_number = 0;

	if(str_crypted_size % 24 == 0)
	{
		blocks_number = str_crypted_size / 24;
	}
	else
	{
		blocks_number = str_crypted_size / 24 + 1;
	}

	d_Etat = malloc(sizeof(char) * blocks_number * 25);

	/* d_Etat = 0 */
	for(int i = 0; i < blocks_number; i++)
	{
		memset(d_Etat[i], 0, 25);
	}

	/* d_Etat = c */
	for(int i = 0; i < blocks_number; i++)
	{
		for(int j = 0; j < 24; j++)
		{
			d_Etat[i][j] = used_state_params[i][j];
		}

		d_Etat[i][24] = '\0';
	}

	/* d_Etat XOR K11 */
	char xor_result;
	for(int i = 0; i < blocks_number; i++)
	{
		for(int j = 0; j < 24; j++)
		{

			xor_result = xor(d_Etat[i][j], used_keys->g_sub_keys[10][j]);
			d_Etat[i][j] = xor_result;
		}	

		d_Etat[i][24] = '\0';
	}

	/* unpresent, xor with Ki keys must be done in reverse,
	 * K10 to K1. */
	for(int i = 0; i < blocks_number; i++)
	{
		for(int j = 10; j >= 0; j--)
		{
			unPermutation(d_Etat[i]);	
			unSubstitution(d_Etat[i]);

			for(int k = 0; k < 24; k++)
			{
				xor_result = xor(d_Etat[i][j], used_keys->g_sub_keys[j][k]);
				d_Etat[i][k] = xor_result;
			}
		}
	}
}

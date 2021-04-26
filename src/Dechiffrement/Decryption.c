#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../headers/Dechiffrement/Decryption.h"

int split_message_dec(char * message)
{
    int taille = strlen(message);
    int nb_blocs;

    if( (taille % 24) == 0)
        nb_blocs =  (taille / 24) ;
    else
        nb_blocs = (taille / 24) + 1;

    int i;
    d_Etat = malloc(nb_blocs * sizeof(char*));

    for(i = 0 ; i < nb_blocs ; i++)
    {
        d_Etat[i] = malloc(25 * sizeof(char));
        memset(d_Etat[i],0,25);
    }

    int cmp = 0;
    int j = 0;
    for(i = 0 ; i < taille ; i++)
    {
        if( i < (taille - 1) )
        {
            if (i % 24 == 0 && cmp == 0 && j != 24)
            {
                d_Etat[cmp][j] = message[i];
                j++;
            }
            else if (i % 24 == 0 && j == 24)
            {
                d_Etat[cmp][j] = '\0';

                j = 0;
                cmp++;
                d_Etat[cmp][j] = message[i];
                j++;

            }
            else
            {
                d_Etat[cmp][j] = message[i];
                j++;
            }
        }
        else {

            if (i % 24 == 23) {
                d_Etat[cmp][j] = message[i];
                j++;
                d_Etat[cmp][j] = '\0';
            } else {
                for (int k = j; k < 24; k++) {
                    d_Etat[cmp][j] = '0';
                    j++;
                }
                d_Etat[cmp][j] = '\0';
            }
        }
    }

    return nb_blocs;
}

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
    int size = strlen(d_Etat);
	for(int i = 0; i < size; i++)
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

void unpresent(Keys *used_keys_params, char *crypted, char *decrypted)
{

    int i,j,k;
	int blocks_number = split_message_dec(crypted);

	/* d_Etat XOR K11 */
	char xor_result;

	for(i = 0 ; i < blocks_number ; i++)
    {
        for(k = 0; k < 24; k++)
        {
            xor_result = xor(d_Etat[i][k], used_keys_params->g_sub_keys[10][k]);
            d_Etat[i][k] = xor_result;
            decrypted[k] = d_Etat[i][k];
        }
        d_Etat[i][24] = '\0';
    }

/* unpresent, xor with Ki keys must be done in reverse, K10 to K1. */
    int cmp_dcrypted = 0 ;
	for(i = 0; i < blocks_number; i++)
	{
		for(j = 9; j >= 0; j--)
		{
			unPermutation(d_Etat[i]);	
			unSubstitution(d_Etat[i]);

			for(k = 0; k < 24; k++)
			{
				xor_result = xor(d_Etat[i][k], used_keys_params->g_sub_keys[j][k]);
				d_Etat[i][k] = xor_result;
			}
		}
	}

	for(i = 0 ; i < blocks_number ; i++)
    {
	    for(j = 0 ; j < 24 ; j++)
        {
            decrypted[cmp_dcrypted] = d_Etat[i][j];
            cmp_dcrypted++;

        }
    }
	decrypted[cmp_dcrypted] = '\0';
	
}

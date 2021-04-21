#include "keys.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Generate a master key of 24 bits. */
void generate_master_key()
{
	srandom(time(NULL));

	for(int i = 0; i < 24; i++)
	{
		g_master_key[i] = random() % 16;
	}
}

char *s_box(char *entry_params)
{
	for(int i = 0; i < 4; i++)
	{
		switch(entry_params[i])
		{
			case 0:
				entry_params[i] = 12;
				break;
			case 1:
				entry_params[i] = 5;
				break;
			case 2:
				entry_params[i] = 6;
				break;
			case 3:
				entry_params[i] = 11;
				break;
			case 4:
				entry_params[i] = 9;
				break;
			case 5:
				entry_params[i] = 0;
				break;
			case 6:
				entry_params[i] = 10;
				break;
			case 7:
				entry_params[i] = 13;
				break;
			case 8:
				entry_params[i] = 3;
				break;
			case 9:
				entry_params[i] = 14;
				break;
			case 10:
				entry_params[i] = 15;
				break;
			case 11:
				entry_params[i] = 8;
				break;
			case 12:
				entry_params[i] = 4;
				break;
			case 13:
				entry_params[i] = 7;
				break;
			case 14:
				entry_params[i] = 1;
				break;
			case 15:
				entry_params[i] = 2;
				break;
		}
	}

	return entry_params;
}

char *Substitution(char *Etat)
{
	char word_i[4];
	char *ret = malloc(32 * sizeof(char));
	int j = 0;

	for(int i = 0; i < 24; i++)
	{
		word_i[j] = Etat[i];
		j++;
			if(j == 4)
			{
				j = 0;
				strcat(ret, s_box(word_i)); //weird behavior, 1 out of 2 times it works
			}
	}

	return ret;	
}

char *p_box(char *entry_params)
{
	int cpt = 0;
	int j = 0;

	for(int i = 0; i < 24; i++)
	{
		entry_params[i] << (j * 6 + cpt);//gcc : no effect I don't know why
		j++;
			if(i == 3 || i == 7 || i == 11 || i == 15 || i == 19)
			{
				j = 0;
				cpt++;
			}
	}

	return entry_params;
}

char *Permuttation(char *Etat)
{
	char *ret = malloc(32 * sizeof(char));

	for(int i = 0; i < 24; i++)
	{
		strcat(ret, p_box(Etat)); //weird behavior	
	}


	return ret;
}

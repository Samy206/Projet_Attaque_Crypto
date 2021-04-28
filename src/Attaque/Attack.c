#include "../../headers/Attaque/Attack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

void init_couple(Couple_strings * couple,char * str1 , char * str2)
{
    if(couple != NULL)
    {
        int size = sizeof(str1) + 1;
        couple->strA = malloc(size);
        couple->strB = malloc(size);

        for(int i = 0 ; i < size ; i++ )
        {
            couple->strA[i] = str1[i];
            couple->strB[i] = str2[i];
        }
        couple->strA[size-1] = '\0';
        couple->strB[size-1] = '\0';
    }
}

void iteration_to_key(char * entry_params, int number)
{
    int exposant = 23;
    int comparator = pow(2,exposant);
    int cmp = 0;

    while(exposant >= 0)
    {
        if (number >= comparator)
        {
            entry_params[cmp] = '1';
            number = number - comparator;
        }
        else
        {
            entry_params[cmp] = '0';
        }
        exposant--;
        cmp++;
        comparator = pow(2, exposant);
    }
}


void * calcul_encryption_A(void * message)
{
    char * input = (char *) (message);
    char *tmp1 = malloc(sizeof(char) * 25);
    Keys *tmp2 = malloc(sizeof(Keys));

    for(int i = 0 ; i < SIZE_ALL/2 ; i++)
    {
	iteration_to_key(tmp1,i);
        init_key(tmp2,tmp1);

	present(tmp2,message,tmp1);
	g_liste_encryption[i] = char_to_double(tmp1);
    }

    free(tmp1);
    free(tmp2);
    free(input);

    pthread_exit(NULL);
}
void * calcul_encryption_B(void * message)
{
    char * input = (char *) (message);
    
    char *tmp1 = malloc(sizeof(char) * 25);
    Keys *tmp2 = malloc(sizeof(Keys));

    for(int i = 0 ; i < SIZE_ALL/2 ; i++)
    {
	iteration_to_key(tmp1,i);
        init_key(tmp2,tmp1);

	present(tmp2,message,tmp1);
	g_liste_encryption[i] = char_to_double(tmp1);
    }

    free(tmp1);
    free(tmp2);
    free(input);

    pthread_exit(NULL);
}
void * calcul_decryption_A(void * crypted)
{
    char * input = (char *) (crypted);
    
    char *tmp1 = malloc(sizeof(char) * 25);
    Keys *tmp2 = malloc(sizeof(Keys));

    for(int i = 0 ; i < SIZE_ALL/2 ; i++)
    {
	iteration_to_key(tmp1,i);
        init_key(tmp2,tmp1);

	unpresent(tmp2,crypted,tmp1);
	g_liste_decryption[i] = char_to_double(tmp1);
    }

    free(tmp1);
    free(tmp2);
    free(input);

    pthread_exit(NULL);

}

void * calcul_decryption_B(void * crypted)
{
    char * input = (char *) (crypted);
    
    char *tmp1 = malloc(sizeof(char) * 25);
    Keys *tmp2 = malloc(sizeof(Keys));

    for(int i = 0 ; i < SIZE_ALL/2 ; i++)
    {
	iteration_to_key(tmp1,i);
        init_key(tmp2,tmp1);

	unpresent(tmp2,crypted,tmp1);
	g_liste_decryption[i] = char_to_double(tmp1);
    }

    free(tmp1);
    free(tmp2);
    free(input);

    pthread_exit(NULL);
}

double char_to_double(char *entry)
{
	if(!entry) 
		return -1;

	double res;
	int entry_size = strlen(entry);
	int cmp = entry_size - 1;

	for(int i = 0; i < entry_size; i++)
	{
		if(entry[i] == '1')
			res += pow(2,cmp);
			cmp--;
	}

return res;
}


void calculate_possibilities(char * message, char * crypted)
{
	pthread_t threads[4];
	int check = 0;

	check = pthread_create(&threads[0],NULL, calcul_encryption_A,message);
	if(check != 0)
	printf("c'est la merde 1\n");

/*	printf("check 1: %d\n",check);
	check = pthread_create(&threads[1],NULL, calcul_encryption_B,message);
	if(check != 0)
	printf("c'est la merde 2\n");

	printf("check 2: %d\n",check);
	check = pthread_create(&threads[2],NULL, calcul_decryption_A,crypted);
	if(check != 0)
	printf("c'est la merde 3\n");

	printf("check 3: %d\n",check);
	check = pthread_create(&threads[3],NULL, calcul_decryption_B,crypted);
	if(check != 0)
        printf("c'est la merde 4\n");
  */ 
	pthread_join(threads[0],NULL); 
//	pthread_join(threads[1],NULL); 
//	pthread_join(threads[2],NULL); 
//	pthread_join(threads[3],NULL); 


/* Partie pour les nuls
 *
	char tmp[25];
    tmp[24] = '\0';
	Keys *tmp3 = malloc(sizeof(Keys));

    for(int i = 0; i < SIZE_ALL; i++)
    {
	
	iteration_to_key(tmp,i);
        init_key(tmp3,tmp);

	present(tmp3,message,tmp);
	g_liste_encryption[i] = char_to_double(tmp);
	unpresent(tmp3,crypted,tmp);
	g_liste_decryption[i] = char_to_double(tmp);
	printf("\nPotit CPU i : %d\n",i);
    }
*/
}



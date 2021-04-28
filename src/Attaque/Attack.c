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

void generate_keys()
{
    char tmp[25];
    tmp[24] = '\0';
    for(int i = 0 ; i < SIZE_ALL ; i++)
    {
        iteration_to_key(tmp,i);
        init_key(&keys[i],tmp);
    }
}

void * calcul_encryption_A(void * message)
{
    char * input = (char *) (message);
    for(int i = 0 ; i < SIZE_ALL/2 ; i++)
    {
        present(&keys[i],input,g_liste_encryption[i]);
        g_liste_encryption[i][24] = '\0';
    }
}
void * calcul_encryption_B(void * message)
{
    char * input = (char *) (message);
    for(int i = SIZE_ALL/2 ; i < SIZE_ALL ; i++)
    {
        present(&keys[i],input,g_liste_encryption[i]);
        g_liste_encryption[i][24] = '\0';
    }
}
void * calcul_decryption_A(void * crypted)
{
    char * input = (char *) (crypted);
    for(int i = 0 ; i < SIZE_ALL/2 ; i++)
    {
        unpresent(&keys[i],input,g_liste_decryption[i]);
        g_liste_decryption[i][24] = '\0';
    }
}
void * calcul_decryption_B(void * crypted)
{
    char * input = (char *) (crypted);
    for(int i = SIZE_ALL/2 ; i < SIZE_ALL ; i++)
    {
        unpresent(&keys[i],input,g_liste_decryption[i]);
        g_liste_decryption[i][24] = '\0';
    }
}


void calculate_possibilities(char * message, char * crypted)
{
    generate_keys();
    pthread_t threads[4];
    int check = 0;

    check = pthread_create(&threads[0],NULL, calcul_encryption_A,message);
    if(check != 0)
        printf("c'est la merde 1\n");

    printf("check 1: %d\n",check);
    check = pthread_create(&threads[1],NULL, calcul_encryption_B,message);
    if(check != 0)
        printf("c'est la merde 2\n");

    printf("check 2: %d\n",check);
    check = pthread_create(&threads[2],NULL, calcul_decryption_A,message);
    if(check != 0)
        printf("c'est la merde 3\n");

    printf("check 3: %d\n",check);
    check = pthread_create(&threads[3],NULL, calcul_decryption_B,message);
    if(check != 0)
        printf("c'est la merde 4\n");
    

    pthread_exit(NULL);
}


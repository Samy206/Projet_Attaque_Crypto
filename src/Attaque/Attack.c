#include "../../headers/Attaque/Attack.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_cmp_couples_keys =  0;

void calculate_possibities(uint24 message,uint24 crypted)
{
    pthread_t threads[2];
    int check;

    check = pthread_create(&threads[0],NULL, calculate_enc,&message);
    check = pthread_create(&threads[1],NULL, calculate_dec,&crypted);


    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
}

void * calculate_enc(void * message)
{
    uint24 * entry = (uint24 *) (message);
    uint24 tmp;
    for(int i = 0 ; i < SIZE ; i++)
    {
        tmp.x = i;
        init_key(&keys[i],tmp);

        g_liste_encryption[i].x = present(&keys[i],*entry).x;
        if(i == SIZE/4)
            printf("25 calcul, i : %d\n",i);
        if(i == SIZE/2)
            printf("50 calcul, i : %d\n",i);
    }
}

void * calculate_dec(void * crypted)
{
    uint24 * entry = (uint24 *) (crypted);
    uint24 tmp;
    for(int i = 0 ; i < SIZE ; i++)
    {
        tmp.x = i;
        init_key(&keys[i],tmp);

        g_liste_decryption[i].x = un_present(&keys[i],*entry).x;
    }
}

int search_collisions()
{
    pthread_t threads[2];
    int check;

    check = pthread_create(&threads[0],NULL, search_high,NULL);
    check = pthread_create(&threads[1],NULL, search_low,NULL);


    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    return g_cmp_couples_keys;
}

void * search_high(void * nothing)
{
    for(int i = SIZE/2 ; i < SIZE ; i++)
    {
        for(int j = 0 ; j < SIZE ; j++)
        {
            if(g_liste_encryption[i].x == g_liste_decryption[j].x)
            {
                couples[g_cmp_couples_keys].indexA = i;
                couples[g_cmp_couples_keys].indexB = j;
                g_cmp_couples_keys++;
            }
        }
    }
}
void * search_low(void * nothing)
{
    for(int i = 0 ; i < SIZE/2 ; i++)
    {
        printf("i : %d\n",i);

        for(int j = 0 ; j < SIZE ; j++)
        {
            if(g_liste_encryption[i].x == g_liste_decryption[j].x)
            {
                couples[g_cmp_couples_keys].indexA = i;
                couples[g_cmp_couples_keys].indexB = j;
                g_cmp_couples_keys++;
            }
        }
    }

}


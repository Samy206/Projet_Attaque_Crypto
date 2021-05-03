#include "../../headers/Attaque/Attack.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_cmp_couples_keys =  0;

int comparator_g_list(const void *p1, const void *p2)
{
	uint24 *arg1 = (uint24 *) p1;
	uint24 *arg2 = (uint24 *) p2;

	if(arg1->x < arg2->x)
	{
		return -1;
	}

	if(arg1->x == arg2->x)
	{
		return 0;
	}
	if(arg1->x > arg2->x)
	{
		return 1;
	}
}

void calculate_possibities(uint24 message,uint24 crypted)
{
    pthread_t threads[4];
    int check;

    check = pthread_create(&threads[0],NULL, calculate_encA,&message);
    check = pthread_create(&threads[1],NULL, calculate_decA,&crypted);
    check = pthread_create(&threads[2],NULL, calculate_encB,&message);
    check = pthread_create(&threads[3],NULL, calculate_decB,&crypted);


    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    pthread_join(threads[2],NULL);
    pthread_join(threads[3],NULL);

	qsort(g_liste_encryption, SIZE, sizeof(uint24), comparator_g_list);
    
	qsort(g_liste_decryption, SIZE, sizeof(uint24), comparator_g_list);

    printf("\n\n --------SEARCHING COLLISIONS----------- \n\n");

}

void * calculate_encA(void * message)
{
    uint24 * entry = (uint24 *) (message);
    uint24 tmp;
    for(int i = 0 ; i < SIZE/2 ; i++)
    {
        tmp.x = i;
        init_key(&keys[i],tmp);

        g_liste_encryption[i].x = present(&keys[i],*entry).x;
        if(i == SIZE/4)
            printf("50 calcul, i : %d\n",i);
    }
}

void * calculate_encB(void * message)
{
    uint24 * entry = (uint24 *) (message);
    uint24 tmp;
    for(int i = SIZE/2 ; i < SIZE ; i++)
    {
        tmp.x = i;
        init_key(&keys[i],tmp);

        g_liste_encryption[i].x = present(&keys[i],*entry).x;
    }
}

void * calculate_decA(void * crypted)
{
    uint24 * entry = (uint24 *) (crypted);
    uint24 tmp;
    for(int i = 0 ; i < SIZE/2 ; i++)
    {
        tmp.x = i;
        init_key(&keys[i],tmp);

        g_liste_decryption[i].x = un_present(&keys[i],*entry).x;
    }
}

void * calculate_decB(void * crypted)
{
    uint24 * entry = (uint24 *) (crypted);
    uint24 tmp;
    for(int i = SIZE/2 ; i < SIZE ; i++)
    {
        tmp.x = i;
        init_key(&keys[i],tmp);

        g_liste_decryption[i].x = un_present(&keys[i],*entry).x;
    }
}


int search_collisions()
{
    pthread_t threads[4];
    int check;

    check = pthread_create(&threads[0],NULL, search_highA,NULL);
    check = pthread_create(&threads[1],NULL, search_lowA,NULL);
    check = pthread_create(&threads[2],NULL, search_highB,NULL);
    check = pthread_create(&threads[3],NULL, search_lowB,NULL);


    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    pthread_join(threads[2],NULL);
    pthread_join(threads[3],NULL);
    return g_cmp_couples_keys;
}

void * search_highA(void * nothing)
{
    for(int i = 0 ; i < SIZE/4 ; i++)
    {
        for(int j = 0 ; j < SIZE ; j++)
        {
	    if(g_liste_encryption[i].x < g_liste_decryption[j].x)
	    {
		    break;
	    }
            if(g_liste_encryption[i].x == g_liste_decryption[j].x)
            {
		    //printf("\n\n \t Collision : %d, %d \n",i,j);
                couples[g_cmp_couples_keys].indexA = i;
                couples[g_cmp_couples_keys].indexB = j;
                g_cmp_couples_keys++;
            }
        }
    }
}

void * search_highB(void * nothing)
{
    for(int i = SIZE/4 ; i < SIZE/2 ; i++)
    {
        for(int j = 0 ; j < SIZE ; j++)
        {
		if(g_liste_encryption[i].x < g_liste_decryption[j].x)
	    	{
		    break;
		}

            if(g_liste_encryption[i].x == g_liste_decryption[j].x)
            {
                couples[g_cmp_couples_keys].indexA = i;
                couples[g_cmp_couples_keys].indexB = j;
                g_cmp_couples_keys++;
            }
        }
    }
}

void * search_lowA(void * nothing)
{
    for(int i = SIZE/2; i < (3*SIZE)/4 ; i++)
    {

        for(int j = 0 ; j < SIZE ; j++)
        {
		if(g_liste_encryption[i].x < g_liste_decryption[j].x)
	    	{
		    break;
	    	}

            if(g_liste_encryption[i].x == g_liste_decryption[j].x)
            {
                couples[g_cmp_couples_keys].indexA = i;
                couples[g_cmp_couples_keys].indexB = j;
                g_cmp_couples_keys++;
            }
        }
    }

}

void * search_lowB(void * nothing)
{
    for(int i = (3*SIZE)/4; i < SIZE ; i++)
    {

        for(int j = 0 ; j < SIZE ; j++)
        {
		if(g_liste_encryption[i].x < g_liste_decryption[j].x)
	    	{
		    break;
	    	}

            if(g_liste_encryption[i].x == g_liste_decryption[j].x)
            {
                couples[g_cmp_couples_keys].indexA = i;
                couples[g_cmp_couples_keys].indexB = j;
                g_cmp_couples_keys++;
            }
        }
    }

}

void check_couples(uint24 message2, uint24 crypted2)
{
	uint24 result;

	for(int i = 0; i < g_cmp_couples_keys; i++)
	{
		result.x = two_present(&keys[couples[i].indexA], &keys[couples[i].indexB], message2).x;

		if(result.x == crypted2.x)
		{
			printf("\n\t Result : (%x,%x) \n",
					keys[couples[i].indexA].master_key.x,
					keys[couples[i].indexB].master_key.x
			);
		}
	}

}

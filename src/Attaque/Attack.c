#include "../../headers/Attaque/Attack.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_cmp_couples_keys =  0;

int comparator_g_list(const void *p1, const void *p2)
{
	const uint24 arg1 = *(const uint24 *) p1;
	const uint24 arg2 = *(const uint24 *) p2;

	return (arg1.x - arg2.x);
}

/* Dual core */
void *calculate_enc_d(void *message)
{
	uint24 *entry = (uint24 *) (message);
	uint24 tmp;

	for(int i = 0; i < SIZE; i++)
	{
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
	    g_liste_encryption[i].x = present(&keys[i],*entry).x;
	}
    pthread_exit(NULL);
}

void *calculate_dec_d(void *crypted)
{
	uint24 *entry = (uint24 *) (crypted);
	uint24 tmp;
	for(int i = 0; i < SIZE; i++)
	{
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
		g_liste_decryption[i].x = un_present(&keys[i],*entry).x;
	}
    pthread_exit(NULL);
}

void calculate_possibilities_dual_core(uint24 message,uint24 crypted)
{
	pthread_t threads[2];
	int check;

	check = pthread_create(&threads[0],NULL, calculate_enc_d,&message);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[1],NULL, calculate_dec_d,&crypted);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[0],NULL);
		if(check != 0){exit(EXIT_FAILURE);}
	check = pthread_join(threads[1],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	qsort(g_liste_encryption, SIZE, sizeof(uint24), comparator_g_list);
	qsort(g_liste_decryption, SIZE, sizeof(uint24), comparator_g_list);


}

void *search_high_d(void *nothing)
{
	int i,j;
	i = 0;
	j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < SIZE/2) && (j < SIZE))
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
            i++;

        else if (g_liste_encryption[i].x > g_liste_decryption[j].x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = i;
            couples[g_cmp_couples_keys].indexB = j;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
            j++;
        }
    }
    return nothing;
}

void *search_low_d(void *nothing)
{
    int i,j;
    i = SIZE/2;
    j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < SIZE) && (j < SIZE))
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
            i++;

        else if (g_liste_encryption[i].x > g_liste_decryption[j].x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = i;
            couples[g_cmp_couples_keys].indexB = j;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
            j++;
        }
    }
    return nothing;
}

int search_collisions_dual_core()
{
	pthread_t threads[2];
	int check;

	check = pthread_create(&threads[0], NULL, search_high_d, NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[1], NULL, search_low_d, NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[0],NULL);
		if(check != 0){exit(EXIT_FAILURE);}
	check = pthread_join(threads[1],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	return g_cmp_couples_keys;
}
/* Dual core */

/* Quad core */
void *calculate_enc_q_A(void *message)
{
	uint24 *entry = (uint24 *) (message);
	uint24 tmp;

	for(int i = 0; i < SIZE/2; i++)
	{
	    tmp.x = i;
	    init_key(&keys[i],tmp);
	    g_liste_encryption[i].x = present(&keys[i],*entry).x;
	}
    return NULL;
}

void *calculate_dec_q_A(void *crypted)
{
	uint24 *entry = (uint24 *) (crypted);
	uint24 tmp;

	for(int i = 0; i < SIZE/2; i++)
	{
		tmp.x = i;
		init_key(&keys[i],tmp);
		g_liste_decryption[i].x = un_present(&keys[i],*entry).x;
	}
    return NULL;
}

void *calculate_enc_q_B(void *message)
{
	uint24 *entry = (uint24 *) (message);
	uint24 tmp;

	for(int i = SIZE/2; i < SIZE; i++)
	{
	    tmp.x = i;
	    init_key(&keys[i],tmp);
	    g_liste_encryption[i].x = present(&keys[i],*entry).x;
	}
    return NULL;
}

void *calculate_dec_q_B(void *crypted)
{
	uint24 *entry = (uint24 *) (crypted);
	uint24 tmp;

	for(int i = SIZE/2; i < SIZE; i++)
	{
		tmp.x = i;
		init_key(&keys[i],tmp);
		g_liste_decryption[i].x = un_present(&keys[i],*entry).x;
	}
    return NULL;
}

void calculate_possibilities_quad_core(uint24 message,uint24 crypted)
{
	pthread_t threads[4];
	int check;

	check = pthread_create(&threads[0],NULL, calculate_enc_q_A,&message);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[1],NULL, calculate_dec_q_A,&crypted);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[2],NULL, calculate_enc_q_B,&message);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[3],NULL, calculate_dec_q_B,&crypted);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[0],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[1],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[2],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[3],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	qsort(g_liste_encryption, SIZE, sizeof(uint24), comparator_g_list);
	qsort(g_liste_decryption, SIZE, sizeof(uint24), comparator_g_list);


}

void *search_high_q_A(void *nothing)
{
    nothing = NULL;
    int i,j;
    i = 0;
    j = 0;
    while (i < SIZE/4 && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
            i++;

        else if (g_liste_encryption[i].x > g_liste_decryption[j].x)
            j++;

        else
        {
            couples[g_cmp_couples_keys].indexA = i;
            couples[g_cmp_couples_keys].indexB = j;
            g_cmp_couples_keys++;
            i++;
            j++;
        }
    }
	pthread_exit(NULL);

}

void *search_high_q_B(void *nothing)
{
    int i,j;
    i = SIZE / 4;
    j = 0;
    while (i < SIZE/2 && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
            i++;

        else if (g_liste_encryption[i].x > g_liste_decryption[j].x)
            j++;

        else
        {
            couples[g_cmp_couples_keys].indexA = i;
            couples[g_cmp_couples_keys].indexB = j;
            g_cmp_couples_keys++;
            i++;
            j++;
        }
    }
    return nothing;
}

void *search_low_q_A(void *nothing)
{
    int i,j;
    i = SIZE/2;
    j = 0;
    while ( (i < 3*SIZE/4)  && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
            i++;

        else if (g_liste_encryption[i].x > g_liste_decryption[j].x)
            j++;

        else
        {
            couples[g_cmp_couples_keys].indexA = i;
            couples[g_cmp_couples_keys].indexB = j;
            g_cmp_couples_keys++;
            i++;
            j++;
        }
    }
    return nothing;
	pthread_exit(NULL);

}

void *search_low_q_B(void *nothing)
{
    int i,j;
    i = 3*SIZE/4;
    j = 0;
    while ( i < SIZE  && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
            i++;

        else if (g_liste_encryption[i].x > g_liste_decryption[j].x)
            j++;

        else
        {
            couples[g_cmp_couples_keys].indexA = i;
            couples[g_cmp_couples_keys].indexB = j;
            g_cmp_couples_keys++;
            i++;
            j++;
        }
    }
    return nothing;
	pthread_exit(NULL);

}

int search_collisions_quad_core()
{
	pthread_t threads[4];
	int check;

	check = pthread_create(&threads[0], NULL, search_high_q_A, NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[1], NULL, search_low_q_A, NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[2], NULL, search_high_q_B, NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_create(&threads[3], NULL, search_low_q_B, NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[0],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[1],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[2],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	check = pthread_join(threads[3],NULL);
		if(check != 0){exit(EXIT_FAILURE);}

	return g_cmp_couples_keys;
}
/* Quad core */

void check_couples(int * tab_index, uint24 message2, uint24 crypted2)
{
	uint24 result;
    uint24 tmp1;
    int cmp = 0;
	for(int i = 0; i < g_cmp_couples_keys; i++)
	{
	    tmp1.x = un_present(&keys[couples[i].indexA],crypted2).x;
		result.x = un_present(&keys[couples[i].indexB], tmp1).x;

		if(result.x == message2.x)
		{
		    tab_index[cmp] = couples[i].indexA;
		    cmp++;
		    tab_index[cmp] = couples[i].indexB;
		    cmp++;

		    printf("couple : (%x , %x)\n",keys[couples[i].indexA].master_key.x,
                                 keys[couples[i].indexB].master_key.x);
		}
	}
	printf("cmp : %d\n",cmp);

}

#include "../../headers/Attaque/Attack.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_cmp_couples_keys =  0;

int comparator_g_list(const void *p1, const void *p2)
{
	const uint24 arg1 = *(const uint24 *) p1;
	const uint24 arg2 = *(const uint24 *) p2;

	if(arg1.x < arg2.x)
	{
		return -1;
	}
	else if(arg1.x > arg2.x)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Dual core */
void *calculate_enc_d(void *message)
{
	uint24 *entry = (uint24 *) (message);
	uint24 tmp;

	for(int i = 0; i < SIZE; i++)
	{
	    tmp.x = i;
	    init_key(&keys[i],tmp);
	    g_liste_encryption[i].x = present(&keys[i],*entry).x;
	}
}

void *calculate_dec_d(void *crypted)
{
	uint24 *entry = (uint24 *) (crypted);
	uint24 tmp;

	for(int i = 0; i < SIZE; i++)
	{
		tmp.x = i;
		init_key(&keys[i],tmp);
		g_liste_decryption[i].x = un_present(&keys[i],*entry).x;
	}
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

	printf("\n\n --------SEARCHING COLLISIONS----------- \n\n");
}

void *search_high_d(void *nothing)
{
	int i,j;
	i = 0;
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
        }
    }
}

void *search_low_d(void *nothing)
{
    int i,j;
    i = SIZE/2;
    j = 0;

    while (i < SIZE && j < SIZE)
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
        }
    }
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

	printf("\n\n --------SEARCHING COLLISIONS----------- \n\n");
}

void *search_high_q_A(void *nothing)
{
	printf("\nStarting searching thread n°1\n");
	pthread_mutex_t m_id;
	pthread_mutex_init(&m_id, NULL);

    int i,j;
    i = 0;
    j = 0;
    while (i < SIZE/4 && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
		i++;
        else
	{	
		if (g_liste_encryption[i].x > g_liste_decryption[j].x)
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
    }
    	printf("\nEnding searching thread n°1\n");
	pthread_exit(NULL);

}

void *search_high_q_B(void *nothing)
{
	printf("\nStarting searching thread n°4\n");
	pthread_mutex_t m_id;
	pthread_mutex_init(&m_id, NULL);

    int i,j;
    i = SIZE/4;
    j = 0;
    while ( (i < SIZE/2)  && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
	{
            i++;
	}
        else
	{	
		if (g_liste_encryption[i].x > g_liste_decryption[j].x)
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
    }
    	printf("\nEnding searching thread n°4\n");
	pthread_exit(NULL);
}

void *search_low_q_A(void *nothing)
{
	printf("\nStarting searching thread n°3\n");
	pthread_mutex_t m_id;
	pthread_mutex_init(&m_id, NULL);

    int i,j;
    i = SIZE/2;
    j = 0;
    while ( (i < 3*SIZE/4)  && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
	{
            i++;
	}
        else
	{	
		if (g_liste_encryption[i].x > g_liste_decryption[j].x)
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
    }
	printf("\nEnding searching thread n°3\n");
	pthread_exit(NULL);

}

void *search_low_q_B(void *nothing)
{
	printf("\nStarting searching thread n°2\n");
	pthread_mutex_t m_id;
	pthread_mutex_init(&m_id, NULL);

    int i,j;
    i = 3*SIZE/4;
    j = 0;
    while ( i < SIZE  && j < SIZE)
    {
        if (g_liste_encryption[i].x < g_liste_decryption[j].x)
	{
            i++;
	}
	else
	{	
		if (g_liste_encryption[i].x > g_liste_decryption[j].x)
            		j++;
        	else
        	{
		printf("\ng_cmp_couples : %d\n",g_cmp_couples_keys);
		pthread_mutex_lock(&m_id);
            	couples[g_cmp_couples_keys].indexA = i;
           	 couples[g_cmp_couples_keys].indexB = j;
           	 g_cmp_couples_keys++;
		pthread_mutex_unlock(&m_id);
		i++;
		j++;
        	}
	}
    }
	printf("\nEnding searching thread n°2\n");
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

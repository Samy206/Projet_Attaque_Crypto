#include "../../headers/Attaque/Attack.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Variable globale du nombre de collisions ( une collision = un couple de clés ).
   Dans le cadre d'utilisation de ce genre de variable on a recours à des mutex afin d'éviter les erreurs de doubles
   écritures/lectures.
*/
int g_cmp_couples_keys =  0;

/*Fonction de comparaison qui nous permet de trier les tableaux "g_liste_encryption" et "g_liste_decryption" à la fin
 des calculs pour effectuer une recherche de collisions plus rapide par la suite. */
int comparator_g_list(const void *p1, const void *p2)
{
	const Element arg1 = *(const Element *) p1;
	const Element arg2 = *(const Element *) p2;

	return (arg1.value.x - arg2.value.x);
}

/* ----------------------------------Dual core------------------------------------ */

/* Stockage du chiffrement de m1 dans "g_liste_encryption" avec la clé utilisée */
void *calculate_enc_d(void *message)
{
	uint24 *entry = (uint24 *) (message);
	uint24 tmp;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
	for(int i = 0; i < SIZE; i++)
	{
        pthread_mutex_lock(&m_id);
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
	    g_liste_encryption[i].value.x = present(&keys[i],*entry).x;
        g_liste_encryption[i].index_key = i;
        pthread_mutex_unlock(&m_id);
	}
    pthread_exit(NULL);
}

/* Stockage du déchiffrement de c1 dans "g_liste_decryption" avec la clé utilisée */
void *calculate_dec_d(void *crypted)
{
    uint24 *entry = (uint24 *) (crypted);
    uint24 tmp;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    for(int i = 0; i < SIZE; i++)
    {
        pthread_mutex_lock(&m_id);
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
        g_liste_decryption[i].value.x = un_present(&keys[i],*entry).x;
        g_liste_decryption[i].index_key = i;
        pthread_mutex_unlock(&m_id);
    }
    pthread_exit(NULL);
}

/*
 Appel des deux fonctions précédentes avec deux threads en leur donnant à l'un le message clair et l'autre le chiffré.
*/
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


	qsort(g_liste_encryption, SIZE, sizeof(Element), comparator_g_list);
	qsort(g_liste_decryption, SIZE, sizeof(Element), comparator_g_list);

}

/* Recherche des collisions entre les indices 0 et SIZE/2 (deux threads) du premier tableau avec le deuxième */
void *search_high_d(void *nothing)
{
	int i,j;
	i = 0;
	j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < SIZE/2) && (j < SIZE))
    {
        if (g_liste_encryption[i].value.x < g_liste_decryption[j].value.x)
            i++;

        else if (g_liste_encryption[i].value.x > g_liste_decryption[j].value.x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = g_liste_encryption[i].index_key;
            couples[g_cmp_couples_keys].indexB = g_liste_decryption[j].index_key;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
        }
    }
    return nothing;
}

/* Recherche des collisions entre les indices SIZE/2 et SIZE-1 (deux threads) du premier tableau avec le deuxième */
void *search_low_d(void *nothing)
{
    int i,j;
    i = SIZE/2;
    j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < SIZE) && (j < SIZE))
    {
        if (g_liste_encryption[i].value.x < g_liste_decryption[j].value.x)
            i++;

        else if (g_liste_encryption[i].value.x > g_liste_decryption[j].value.x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = g_liste_encryption[i].index_key;
            couples[g_cmp_couples_keys].indexB = g_liste_decryption[j].index_key;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
        }
    }
    return nothing;
}


/*
 Appel des deux fonctions précédentes avec deux threads et stockage des collisions dans le tableau "couples".
*/
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


/* ----------------------------------Quad core------------------------------------ */

/*Même principe que pour les dual cores mais division du travail en quatre :
    -Calcul chiffrement et déchiffrement :
        Deux fonctions pour chacun des buts, allant donc de 0 à SIZE/2 pour les fonctions_A,
                                             et de SIZE/2 à SIZE pour les fonctions_B.

    -Recherche de collisions :
       Quatre fonctions de :
         0 à SIZE/4
         SIZE/4 à SIZE/2
         SIZE/2 à 3*SIZE/4
         3*SIZE/4 à SIZE
        Sur le tableau "g_liste_encryption" comparé à "g_liste_decryption" dans son entièreté.
*/

void *calculate_enc_q_A(void *message)
{
    uint24 *entry = (uint24 *) (message);
    uint24 tmp;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    for(int i = 0; i < SIZE/2; i++)
    {
        pthread_mutex_lock(&m_id);
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
        g_liste_encryption[i].value.x = present(&keys[i],*entry).x;
        g_liste_encryption[i].index_key = i;
        pthread_mutex_unlock(&m_id);
    }
    pthread_exit(NULL);
}

void *calculate_dec_q_A(void *crypted)
{
    uint24 *entry = (uint24 *) (crypted);
    uint24 tmp;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    for(int i = 0; i < SIZE/2; i++)
    {
        pthread_mutex_lock(&m_id);
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
        g_liste_decryption[i].value.x = un_present(&keys[i],*entry).x;
        g_liste_decryption[i].index_key = i;
        pthread_mutex_unlock(&m_id);
    }
    pthread_exit(NULL);
}

void *calculate_enc_q_B(void *message)
{
    uint24 *entry = (uint24 *) (message);
    uint24 tmp;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    for(int i = SIZE/2; i < SIZE; i++)
    {
        pthread_mutex_lock(&m_id);
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
        g_liste_encryption[i].value.x = present(&keys[i],*entry).x;
        g_liste_encryption[i].index_key = i;
        pthread_mutex_unlock(&m_id);
    }
    pthread_exit(NULL);
}

void *calculate_dec_q_B(void *crypted)
{
    uint24 *entry = (uint24 *) (crypted);
    uint24 tmp;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    for(int i = SIZE/2; i < SIZE; i++)
    {
        pthread_mutex_lock(&m_id);
        if(keys[i].status != 1) {tmp.x = i; init_key(&keys[i],tmp);}
        g_liste_decryption[i].value.x = un_present(&keys[i],*entry).x;
        g_liste_decryption[i].index_key = i;
        pthread_mutex_unlock(&m_id);
    }
    pthread_exit(NULL);
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

	qsort(g_liste_encryption, SIZE, sizeof(Element), comparator_g_list);
	qsort(g_liste_decryption, SIZE, sizeof(Element), comparator_g_list);


}

void *search_high_q_A(void *nothing)
{
    int i,j;
    i = 0;
    j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < SIZE/4) && (j < SIZE))
    {
        if (g_liste_encryption[i].value.x < g_liste_decryption[j].value.x)
            i++;

        else if (g_liste_encryption[i].value.x > g_liste_decryption[j].value.x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = g_liste_encryption[i].index_key;
            couples[g_cmp_couples_keys].indexB = g_liste_decryption[j].index_key;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
        }
    }
	pthread_exit(NULL);

}

void *search_high_q_B(void *nothing)
{
    int i,j;
    i = SIZE/4;
    j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < SIZE/2) && (j < SIZE))
    {
        if (g_liste_encryption[i].value.x < g_liste_decryption[j].value.x)
            i++;

        else if (g_liste_encryption[i].value.x > g_liste_decryption[j].value.x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = g_liste_encryption[i].index_key;
            couples[g_cmp_couples_keys].indexB = g_liste_decryption[j].index_key;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
        }
    }
    pthread_exit(NULL);
}

void *search_low_q_A(void *nothing)
{
    int i,j;
    i = SIZE/2;
    j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < (3*SIZE/4)) && (j < SIZE))
    {
        if (g_liste_encryption[i].value.x < g_liste_decryption[j].value.x)
            i++;

        else if (g_liste_encryption[i].value.x > g_liste_decryption[j].value.x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = g_liste_encryption[i].index_key;
            couples[g_cmp_couples_keys].indexB = g_liste_decryption[j].index_key;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
        }
    }
	pthread_exit(NULL);

}

void *search_low_q_B(void *nothing)
{
    int i,j;
    i = (3*SIZE)/4;
    j = 0;
    pthread_mutex_t m_id;
    pthread_mutex_init(&m_id, NULL);
    while ((i < SIZE) && (j < SIZE))
    {
        if (g_liste_encryption[i].value.x < g_liste_decryption[j].value.x)
            i++;

        else if (g_liste_encryption[i].value.x > g_liste_decryption[j].value.x)
            j++;

        else
        {
            pthread_mutex_lock(&m_id);
            couples[g_cmp_couples_keys].indexA = g_liste_encryption[i].index_key;
            couples[g_cmp_couples_keys].indexB = g_liste_decryption[j].index_key;
            g_cmp_couples_keys++;
            pthread_mutex_unlock(&m_id);
            i++;
        }
    }
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


/* ----------------------------------Vérification résultats------------------------------------ */



/*
 Vérification :
     Teste les couples de collisions obtenus suite à la recherche sur le premier couple clair/chiffré (m1,c1) et
     affiche ceux qui fonctionnent aussi pour (m2,c2).
*/
void check_couples(uint24 message2, uint24 crypted2)
{
	uint24 result;

	for(int i = 0; i < g_cmp_couples_keys; i++)
	{
	    result.x = two_present(&keys[couples[i].indexA],&keys[couples[i].indexB],message2).x;

		if(result.x == crypted2.x)
			printf("\n Found keys : (%x,%x) \n",keys[couples[i].indexA].master_key.x,
                                                keys[couples[i].indexB].master_key.x
                                                );

	}
	printf("\n");

}


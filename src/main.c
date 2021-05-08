#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include "../headers/Dechiffrement/Decryption.h"
#include "../headers/Attaque/Attack.h"
#include "Attaque/Count_cores.c"
#include <time.h>
#include <pthread.h>


void search(int *tableau1, int * tableau2)
{
    int i= 0 ;
    int j = 0 ;
    int cmp = 0;

    while ((i < 20) && (j < 20))
    {
        printf("i : %d | j : %d, cmp : %d\n",tableau1[i],tableau2[j],cmp);
        if (tableau1[i] < tableau2[j])
            i++;

        else if (tableau1[i] > tableau2[j])
            j++;

        else
        {
            cmp++;
            i++;
        }
    }
    printf("nb_elements_communs : %d\n",cmp);
}

void test_keys(Couple_keys * tableau, int cmp)
{
    uint24 messageA, cryptedA, messageB, cryptedB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    messageB.x = 0xDB4819;
    cryptedB.x = 0x75D43B;


    Key * keyA = malloc(sizeof(Key));
    Key * keyB = malloc(sizeof(Key));
    uint24 tmp;
    for(int i = 0 ; i < cmp ; i++)
    {
        tmp.x = tableau[i].indexA;
        init_key(keyA,tmp);
        tmp.x = tableau[i].indexB;
        init_key(keyB,tmp);

        if( ((two_present(keyA,keyB,messageA).x) == cryptedA.x) && ((two_present(keyA,keyB,messageB).x) == cryptedB.x))
            printf("heureux gagnant : (%x,%x) \n",keyA->master_key.x,keyB->master_key.x);
    }


}

void full_program()
{
    Couple_keys couple_check[5];
    double time_spent = 0.0;

    uint24 messageA, messageB, cryptedA, cryptedB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    messageB.x = 0xDB4819;
    cryptedB.x = 0x75D43B;

    int nb;
    int number_configured_procs = 2;
    clock_t begin = clock();
    printf("\n----Dual core detected----\n");
    calculate_possibilities_dual_core(messageA, cryptedA);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on calcul----\n", time_spent);
    time_spent = 0;
    begin =  clock();
    nb = search_collisions_dual_core();
    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on search collisions : %d---\n", time_spent,nb);
    time_spent = 0;
    begin = clock();


    nb = check_couples(couple_check,messageB,cryptedB);
    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on checking couples----\n", time_spent);
    test_keys(couple_check,nb);
    

    //Couples_check Dual Core
/*    struct check_couples_struct threads_params[2];
    pthread_t t_couples[2];
    pthread_create(&t_couples[0], NULL, check_couples_d_A, &threads_params[0]);
    pthread_create(&t_couples[1], NULL, check_couples_d_B, &threads_params[1]);

    void *nb1 = malloc(sizeof(int));
    void *nb2 = malloc(sizeof(int));

    pthread_join(t_couples[0], nb1);
    pthread_join(t_couples[1], nb2);
    nb1 = (int *) nb1;
    nb2 = (int *) nb2;
    //test_keys(couple_check,nb);

*/
/*
    //Couples_check Quad Core
    struct check_couples_struct threads_params[4];
    pthread_t t_couples[4];
    pthread_create(&t_couples[0], NULL, check_couples_q_A, &threads_params[0]);
    pthread_create(&t_couples[1], NULL, check_couples_q_B, &threads_params[1]);
    pthread_create(&t_couples[2], NULL, check_couples_q_C, &threads_params[2]);
    pthread_create(&t_couples[3], NULL, check_couples_q_D, &threads_params[3]);

    pthread_join(t_couples[0], NULL);
    pthread_join(t_couples[1], NULL);
    pthread_join(t_couples[2], NULL);
    pthread_join(t_couples[3], NULL);

    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on checking couples----\n", time_spent);
*/    
}

void full_program_q()
{
    Couple_keys couple_check[5];
    double time_spent = 0.0;

    uint24 messageA, messageB, cryptedA, cryptedB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    messageB.x = 0xDB4819;
    cryptedB.x = 0x75D43B;

    int nb;
    int number_configured_procs = 4;
    clock_t begin = clock();
    printf("\n----Quad core detected----\n");
    calculate_possibilities_quad_core(messageA, cryptedA);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on calcul----\n", time_spent);
    time_spent = 0;
    begin =  clock();
    nb = search_collisions_quad_core();
    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on search collisions : %d---\n", time_spent,nb);
    time_spent = 0;
    begin = clock();


    nb = check_couples(couple_check,messageB,cryptedB);
    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on checking couples----\n", time_spent);
    test_keys(couple_check,nb);
    

    //Couples_check Dual Core
/*    struct check_couples_struct threads_params[2];
    pthread_t t_couples[2];
    pthread_create(&t_couples[0], NULL, check_couples_d_A, &threads_params[0]);
    pthread_create(&t_couples[1], NULL, check_couples_d_B, &threads_params[1]);

    void *nb1 = malloc(sizeof(int));
    void *nb2 = malloc(sizeof(int));

    pthread_join(t_couples[0], nb1);
    pthread_join(t_couples[1], nb2);
    nb1 = (int *) nb1;
    nb2 = (int *) nb2;
    //test_keys(couple_check,nb);

*/
/*
    //Couples_check Quad Core
    struct check_couples_struct threads_params[4];
    pthread_t t_couples[4];
    pthread_create(&t_couples[0], NULL, check_couples_q_A, &threads_params[0]);
    pthread_create(&t_couples[1], NULL, check_couples_q_B, &threads_params[1]);
    pthread_create(&t_couples[2], NULL, check_couples_q_C, &threads_params[2]);
    pthread_create(&t_couples[3], NULL, check_couples_q_D, &threads_params[3]);

    pthread_join(t_couples[0], NULL);
    pthread_join(t_couples[1], NULL);
    pthread_join(t_couples[2], NULL);
    pthread_join(t_couples[3], NULL);

    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on checking couples----\n", time_spent);
*/    
}


void test_encryption()
{
    uint24 messageA, key_testA, cryptedA, key_testB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    key_testA.x = 0XF5417;
    key_testB.x = 0XD79BA;

    Key keyA,keyB;
    init_key(&keyA,key_testA);
    init_key(&keyB,key_testB);

    printf("crypting %x with %x :   %x\n",messageA.x,key_testA.x,present(&keyA,messageA).x);

    uint24 tmp,tmp1;
    tmp.x = two_present(&keyA,&keyB,messageA).x;
    printf("double crypting %x with (%x,%x) : %x\n",messageA.x,key_testA.x,key_testB.x,tmp.x);

    printf("decrypting %x with %x : %x\n",tmp.x,key_testB.x, un_present(&keyB,tmp).x);
    tmp1.x = un_present(&keyA,un_present(&keyB,tmp)).x;

    printf("back to original message %x \n",tmp1.x);

}

void test_keys_sujet()
{
    Key key1;
    uint24 k1;
    k1.x = 0;
    init_key(&key1,k1);
    for(int i = 0 ; i < 11 ; i++)
    {
        printf("subkeys %d : %x\n",i,key1.sub_keys[i].x);
    }

}

int main()
{
   
    printf("\nPlease enter a number of threads : (2 or 4)\n");
    int *q_th = malloc(sizeof(int));
    scanf("%d", q_th);

    if(*q_th == 2)
    {
        full_program();
    return 0;
    }

    if(*q_th == 4)
    {
        full_program_q();
    return 0;
    }

    printf("\nError on number of threads\n");
return -1;
}

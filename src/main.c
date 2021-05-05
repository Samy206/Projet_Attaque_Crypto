#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include "../headers/Dechiffrement/Decryption.h"
#include "../headers/Attaque/Attack.h"
#include "Attaque/Count_cores.c"
#include <time.h>

void test_keys(int A, int B)
{
    uint24 messageA, cryptedA, messageB, cryptedB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    messageB.x = 0xDB4819;
    cryptedB.x = 0x75D43B;

    uint24 key1,key2;
    key1.x = A;
    key2.x = B;

    Key keyA,keyB;
    init_key(&keyA,key1);
    init_key(&keyB,key2);

    uint24 res;
    res.x = two_present(&keyA,&keyB,messageA).x;

    if(res.x != cryptedA.x)
        printf("not done yet A \n");

    else
    {
        res.x = two_present(&keyA,&keyB,messageB).x;
        if(res.x != cryptedB.x)
            printf("not done yet B\n");
    }
}

void full_program(int * tableau)
{
    double time_spent = 0.0;

    uint24 messageA, messageB, cryptedA, cryptedB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    messageB.x = 0xDB4819;
    cryptedB.x = 0x75D43B;

    int nb;
    int number_configured_procs = 2;
    clock_t begin = clock();
    switch(number_configured_procs)
    {
        case 2:
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
            break;

        case 4:
            printf("\n Quad core detected \n");
            calculate_possibilities_quad_core(messageA, cryptedA);
            nb = search_collisions_quad_core();
            printf("nb_collision : %d\n",nb);
            break;

        default:
            printf("\n Using dual core by default \n");
            calculate_possibilities_dual_core(messageA, cryptedA);
            nb = search_collisions_dual_core();
            printf("nb_collision : %d\n",nb);
            break;
    }

    check_couples(tableau,messageB,cryptedB);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n----The elapsed time is %f seconds on checking couples----\n", time_spent);
}

int main()
{
    int tableau[10];
    full_program(tableau);
    test_keys(keys[tableau[0]].master_key.x,keys[tableau[1]].master_key.x);
}

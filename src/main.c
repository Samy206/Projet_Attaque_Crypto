#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include "../headers/Dechiffrement/Decryption.h"
#include "../headers/Attaque/Attack.h"
#include <time.h>
#include <pthread.h>


void full_program()
{

    uint24 messageA, messageB, cryptedA, cryptedB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    messageB.x = 0xDB4819;
    cryptedB.x = 0x75D43B;

    calculate_possibilities_dual_core(messageA, cryptedA);
    search_collisions_dual_core();
    check_couples(messageB,cryptedB);
}

void full_program_q()
{
    uint24 messageA, messageB, cryptedA, cryptedB;
    messageA.x = 0x684D0F;
    cryptedA.x = 0x66FB2E;

    messageB.x = 0xDB4819;
    cryptedB.x = 0x75D43B;

    calculate_possibilities_quad_core(messageA, cryptedA);
    search_collisions_quad_core();
    check_couples(messageB,cryptedB);
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

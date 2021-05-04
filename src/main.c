#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"
#include "../headers/Dechiffrement/Decryption.h"
#include "../headers/Attaque/Attack.h"
#include "Attaque/Count_cores.c"

int main()
{
    uint24 messageA, messageB, cryptedA, cryptedB;
    messageA.x = 0xDF2AC3;
    messageB.x = 0x4F10D3;
    cryptedA.x = 0x7313A2;
    cryptedB.x = 0x1CF0CC;
    int nb;

    int number_configured_procs = 4;//getNumberOfCores();

    switch(number_configured_procs)
    {
        case 2:
		printf("\n Dual core detected \n");
		calculate_possibilities_dual_core(messageA, cryptedA);
		nb = search_collisions_dual_core();
		break;

        case 4:
		printf("\n Quad core detected \n");
		calculate_possibilities_quad_core(messageA, cryptedA);
		nb = search_collisions_quad_core();
		break;
	
	default: /* Case of mono core ? Who is in ancient World ? */
		printf("\n Using dual core by default \n");
		calculate_possibilities_dual_core(messageA, cryptedA);
		nb = search_collisions_dual_core();
		break;
    }

    printf("\n Check couples starting \n");
    check_couples(messageB,cryptedB);
}

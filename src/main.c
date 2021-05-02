#include <stdio.h>
#include <stdlib.h>
#include "../headers/Chiffrement/keys.h"
#include "../headers/Chiffrement/Encryption.h"


int main()
{

    uint24 message, cle;
    message.x = 0xFFF000;
    permutation(&message);
    printf("zero : %x\n",message.x);


}
#include "../../headers/Chiffrement/keys.h"
#include <stdio.h>
#include "../../headers/Usefull_tables.h"
#include <stdint.h>
#include <math.h>

/* tableau s_box où des indices sont changés en leur équivalent comme spécifié dans le sujet */
const uint4 s_box[16] = { {12} , {5} , {6} , {11} , {9} , {0} ,
                          {10} , {13} , {3} , {14} , {15}, {8} , {4},
                          {7} , {1} , {2}
                        };

/* Initialisation d'une clé maître qui copie la valeur passée en paramètres et qui à partir de celle-ci appelle le
 key_schedule.
 Ici le statut nous informe de l'état des sous-clés (1 -> initialisées ; 0 -> non )
*/
void init_key(Key * key, uint24 value)
{
    key->master_key.x = value.x;
    key_schedule(key);
    key->status = 1;
}

/* Algorithme de cadencement de clés :
     Conformément à l'énoncé, on établit un registre de 80 bits et on le manipule afin d'obtenir toutes les sous-clés.
     Les manipulations de bits qui sont effectuées dans la boucle sont spécifiques à la structure uint80 qui contient
     deux entiers non signés de 40 bits.

     *Étape 1 : Nous nous sommes basés sur ce calcul pour le pivot du registre

        [ K79-.... | ....-K0 ]   qui devient [ K18-K0|K79-K59 | K58-K19 ]
            high       low                         high           low

     *Étape 2 :
        -Assez simplement, on décale la partie haute de 36 positions ( 40 - 4 bits) vers la droite afin d'avoir
            les quatre bits de poids fort.
        -On stocke la valeur obtenue par le décalage précédent dans entre_sbox qui est tout de suite modifiée.
        -Pour finir on met les bits de poids fort de la partie haute à 0 et on effectue un OR avec entry_sbox décalée
          de 36 positions vers la gauche.

     *Étape 3 : Le K19 n'est jamais modifié puisque la boucle va jusqu'à i = 10; or 10 s'écrit uniquement sur 4
        bits, de ce fait on ne travaille que sur les bits de K18 à K15.
        -On décale la partie basse vers la gauche de 21 puis vers la droite de 36 afin de ne conserver que les bits
        de K18 jusqu'à k15.
        -On effectue un xor avec le tour i sur 5 bits et on enregistre cette valeur.
        -On met les bits de K18 à K15 à 0 dans la variable backup et on effectue un OR avec le résultat de l'opération
        précédente ( qui est remise à sa place, donc décalée de 15 positions vers la gauche ).
*/


void key_schedule(Key * key)
{
    uint80 k_register;

    /* Copie de la clé sur 24 bits dans le registre de 80 bits */
    k_register.high = (key->master_key.x);
    k_register.high <<= 16;
    k_register.low = 0;

    uint80 backup ;    // Variable "copie" de K_register afin de manipuler plus aisément les bits

    uint4 entry_sbox;  // Variable temporaire qui entre dans la s_box et qui en sort une autre valeur

    /*Sauvegarde de la sous-clé dans la structure*/
    key->sub_keys[0].x = 0;

    for(int i = 1 ; i < 11 ; i++)
    {

        backup.high = k_register.high;
        backup.low = k_register.low;

        /* Étape 1 : pivot de 61 bits */
        k_register.high = 0;
        k_register.high |= (k_register.low << 21);
        k_register.high |= (backup.high >> 19);

        k_register.low >>= 19;
        backup.high <<= 21;
        k_register.low |= backup.high ;
        backup.low = k_register.low;

        /*Étape 2 : s_box[ K79, K78, K77, K76 ]*/
        entry_sbox.x = 0;
        entry_sbox.x |= s_box[k_register.high >> 36].x;
        k_register.high &= 0x0FFFFFFFFF;
        k_register.high |= ((uint64_t) (entry_sbox.x) << 36);

        /*Étape 3 : xor des 5 bits de 19 à 15 avec le tour i*/
        k_register.low <<= 21;
        k_register.low >>= 36;
        k_register.low ^= i;
        k_register.low <<= 15;
        k_register.low |= (backup.low & 1099511136255);


        /*Sauvegarde de la sous-clé dans la structure*/
        key->sub_keys[i].x = 0  ;
        key->sub_keys[i].x |= (k_register.low >> 16);
    }
}

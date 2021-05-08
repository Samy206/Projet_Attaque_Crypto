#include "../../headers/Chiffrement/Encryption.h"
#include "../../headers/Usefull_tables.h"
#include "../../headers/Chiffrement/keys.h"
#include <stdlib.h>
#include <stdio.h>

/* tableau p_box où des indices sont changés en leur équivalent comme spécifié dans le sujet */
const int p_box[24] = { 0 , 6 , 12 , 18 , 1 , 7 , 13 , 19 , 2 , 8 , 14 , 20 , 3, 9 , 15 , 21 , 4 , 10 , 16 , 22 ,
                        5 ,11 , 17 , 23 };


/*Fonctionnement d'une itération de la boucle :
    Soit le tour i = 2 (là ou on traite de le 3eme bit de poids fort) par exemple :

        -Nous déclalons la variable state de 21 positions vers la droite, on a donc le bit dont on veut s'occuper au
          début.

        -Nous effectuons un AND avec 0x1 de façon à ne garder que celui-ci et mettre les autres à 0.

        -Nous enchaînons avec un OR avec une nouvelle variable 'permutation' qui est complétée au fur et à mesure. Ce
          qui nous assure qu'à chaque OR avec un nouveau bit à déplacer, celui-ci est à 0.

        -Nous finissons par déclaler le bit de poids faible vers la gauche de "23 - p_box[i]" positions pour le
          remettre à la bonne place.
*/
void permutation(uint24 * state)
{
    uint24 source;
    source.x = state->x;
    uint24 permutation;
    permutation.x = 0 ;
    int i,distance;
    for (i=0; i<24; i++){
        distance = 23 - i;
        permutation.x = permutation.x | ((source.x >> distance & 0x1) << (23 - p_box[i]));
    }
    state->x = permutation.x;
}


/*Fonctionne plus ou moins de la même manière que la fonction de permuttation à quelques différences près :
    -Quand on décale quatres bits pour les mettre au début (bit de poids faibles), on effectue un décalage à gauche afin
    de les remettre à la même place. ( Cela suite à un passage dans la s_box )
*/
void substitution(uint24 * state)
{
    uint4 entry_sbox;
    uint24 tmp ;
    tmp.x = 0;
    int offset = 4;
    int shift;

    while(offset <= 24)
    {
        entry_sbox.x = 0;
        shift = 24 - offset;

        entry_sbox.x |= (state->x >> shift & 0x00000F);
        entry_sbox.x = s_box[entry_sbox.x].x;
        tmp.x <<= 4 ;
        tmp.x |= entry_sbox.x;

        offset += 4;
    }

    state->x = tmp.x ;
}


/*Le chiffrement est conforme à ce qui a été demandé dans l'énoncé, les étapes sous-jacentes ont déjà été expliquées plus
  haut, cette fonction ne consiste qu'à les appeler dans le bon ordre*/
uint24 present(Key * key, uint24 message)
{
    uint24 state;
    state.x = message.x;

    for(int i = 0 ; i < 10 ; i++)
    {
        state.x = state.x ^ key->sub_keys[i].x;
        substitution(&state);
        permutation(&state);
    }

    state.x = state.x ^ key->sub_keys[10].x;
    return state;
}

/*Present appliqué deux fois*/
uint24 two_present(Key * keyA, Key * keyB, uint24 message)
{
    uint24 tmp,tmp2;
    tmp.x  = present(keyA,message).x;
    tmp2.x = present(keyB,tmp).x;
    return tmp2;
}

#ifndef KEYS
#define KEYS

#include <stdint.h>

/* Structures d'entiers non signés,
 * usages pour le registre K, les clés. */
typedef struct uint24
{
    unsigned int x:24;
}uint24;

typedef struct
{
    unsigned int x:4;
}uint4;

typedef struct
{
    unsigned int x:5;
}uint5;

/* Structure spéciale, contenant le registre de 80 bits
 * Partie haute de K79 à K40.
 * Partie basse de K39 à K0.
*/
typedef struct
{
    unsigned long int high:40;
    unsigned long int low:40;
}uint80;

/* Structure contenant la clé maître
 * et ses sous-clés associées. */
typedef struct
{
    uint24 master_key;
    uint24 sub_keys[11];
    int status;
}Key;

/* Initialise la structure Key. */
void init_key(Key * key, uint24 value);

/* Algorithme de cadencement de clés. */
void key_schedule(Key * key);


#endif

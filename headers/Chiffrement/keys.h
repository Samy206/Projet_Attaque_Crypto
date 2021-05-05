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

typedef struct uint80
{
    unsigned long int x:64;
    unsigned short int y:16;
}uint80;

/* Structure contenant la clé maître
 * et ses sous-clés associées. */
typedef struct
{
    uint24 master_key;
    uint24 sub_keys[11];
}Key;

/* Initialise la structure Key. */
void init_key(Key * key, uint24 value);

/* Algorithme de cadencement de clés. */
void key_schedule(Key * key);

#endif

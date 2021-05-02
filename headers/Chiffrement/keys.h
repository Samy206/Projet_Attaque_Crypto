#ifndef KEYS
#define KEYS

typedef struct
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

typedef struct
{
    unsigned long int x:64;
    unsigned long int y:16;
}uint80;


typedef struct
{
    uint24 master_key;
    uint24 sub_keys[11];
}Key;


void init_key(Key * key, uint24 value);
void key_schedule(Key * key);


#endif

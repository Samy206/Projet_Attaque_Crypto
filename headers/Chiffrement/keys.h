#ifndef _H_KEYS
#define _H_KEYS

typedef struct Keys
{
    char g_master_key[25];
    char g_sub_keys[11][25];
}
Keys;

void generate_master_key(Keys *keys);
void key_schedule_algorithm(Keys *keys);
void init_key(Keys *keys,char * master_key);

int binary_to_decimal(char *entry_params);
void decimal_to_binary(int number,char *entry_params);

#endif

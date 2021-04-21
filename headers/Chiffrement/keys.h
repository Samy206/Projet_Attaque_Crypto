#ifndef _H_KEYS
#define _H_KEYS


char g_master_key[24];
char g_sub_keys[11][24];
char Etat[23];

void generate_master_key();
void key_schedule_algorithm();

char *s_box(char *entry_params);
char *Substitution(char *Etat);

char *p_box(char *entry_params);
char *Permuttation(char *Etat);

#endif

#ifndef TABLES
#define TABLES

#include "Chiffrement/keys.h"

/* Tableau représentant la S-box. */
extern const uint4 s_box[16];

/* Tableau réciproque de la S-box. */
extern const uint4 un_s_box[16];

/* Tableau repésentant la P-box. */
extern const int p_box[24];

#endif

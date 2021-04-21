#include "keys.h"

#include <stdio.h>

int main()
{
	generate_master_key();
	Substitution(g_master_key);
	Permuttation(g_master_key);
	return 0;
}

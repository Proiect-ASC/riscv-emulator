#include <stdlib.h>
#include <string.h>
#include "utils.h"

void check_file(FILE* f, const char* file_name)
{
	if(f == NULL)
	{
		fprintf(f, "[ERROR] could not oopen file: %s\n", file_name);
		exit(1);
	}
}



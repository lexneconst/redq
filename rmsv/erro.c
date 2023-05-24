#include "rmsvw.h"

void cprint(char *message)
{
	printf("> %s \n", message);
}
void fatal(char *message,char *file,uint32_t line, uint32_t statusid)
{
	printf("fatal: %d,(%s,%d) ,%s \n", statusid, file, line, message);
}



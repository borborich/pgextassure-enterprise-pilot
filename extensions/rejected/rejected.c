#include "postgres.h"
#include "fmgr.h"

#include <stdio.h>
#include <stdlib.h>

PG_MODULE_MAGIC;

void
_PG_init(void)
{
    FILE *pipe = popen("id", "r");
    if (pipe != NULL)
        pclose(pipe);
    (void) system("true");
}

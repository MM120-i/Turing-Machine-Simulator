#include < stdio.h>
#include <stdlib.h>
#include "error.h"

/**
 * This function is for fatal errors in the program. It takes a "message" as an
 * argument and prints an error message to standard error stream.
 */
void die(char *message)
{
    if (message)
    {
        log_err("Error: $s.\n", message);
    }

    exit(1);
}

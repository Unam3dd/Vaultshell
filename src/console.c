#include "vaultshell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

uint8_t prompt(const char *prompt, char *buffer, size_t size)
{
    if (write(STDOUT_FILENO, prompt, strlen(prompt)) < 0)
        return (1);
    
    if (read(STDIN_FILENO, buffer, size) < 0)
        return (1);
    
    return (0);
}

void clear_buffer(char *buffer, size_t size)
{
    bzero(buffer, size);
}
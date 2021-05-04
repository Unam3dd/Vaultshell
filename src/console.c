#include "vaultshell.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

uint8_t prompt(prompt_info_t *prompt, char *buffer, size_t size)
{
    char p[0x100] = {0};

    sprintf(p, "\033[38;5;82m%s\033[94m@\033[38;5;82m%s:\033[94m%s\033[38;5;82m$ \033[00m", prompt->user, prompt->host, prompt->path);

    if (write(STDOUT_FILENO, p, strlen(p)) < 0)
        return (1);
    
    if (read(STDIN_FILENO, buffer, size) < 0)
        return (1);
    
    return (0);
}

uint8_t set_prompt_info(prompt_info_t *prompt, char *username, char *host, char *path)
{
    
    prompt->user = username;
    prompt->host = host;
    prompt->path = path;

    return (0);
}

void clear_buffer(char *buffer, size_t size)
{
    bzero(buffer, size);
}
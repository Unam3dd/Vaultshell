#include "vaultshell.h"
#include <string.h>

char *append_chr(char *buffer, char *src, size_t size_buffer)
{
    char *tmp = buffer;

    while (*tmp)
        tmp++;
    
    while (size_buffer-- && *src)
        *(tmp)++ = *(src)++;
    
    *tmp = 0;

    return (buffer);
}

uint32_t split_command(char *buffer, char **command, char *delim)
{
    replace_chr(buffer, '"', ' ');
    char *token = strtok(buffer, delim);
    size_t i = 0;

    while (token) {
        command[i++] = token;
        token = strtok(NULL, delim);
    }

    command[i] = NULL;

    return (i);
}

char *replace_chr(char *buffer, char old, char new)
{
    char *tmp = buffer;

    while (*tmp) {

        if (*tmp == old)
            *tmp = new;
        
        tmp++;
    }

    return (buffer);
}

uint32_t is_pipe(char *buffer)
{
    uint32_t n = 0;

    while (*buffer)
        if (*(buffer)++ == '|')
            n++;
    
    return (n);
}

uint32_t is_redirection(char *buffer)
{
    uint32_t n = 0;

    while (*buffer) {
        if (*buffer == '>' || *buffer == '<')
            n++;
        
        buffer++;
    }

    return (n);
}

uint8_t is_redirection_append(char *buffer)
{
    while (*buffer) {
        if ((buffer[0] == '>' && buffer[1] == '>') || 
        (buffer[0] == '<' && buffer[1] == '<'))
            return (1);
        
        buffer++;
    }
    
    return (0);
}

char *erase_str(char *str, char chr, size_t len)
{
    char *origin = str, *tmp = NULL, *save = NULL;

    while (*origin) {
        
        if (*origin == chr) {
            tmp = origin + 1;
            save = origin;

            while (*origin) {
                *(origin)++ = *(tmp)++;
            }

            origin = save;
        }

        origin++;
    }

    return (str);
}
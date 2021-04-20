#include "vaultshell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t cd(char **command)
{
    if (command[1] == NULL) {
        chdir(getenv("HOME"));
        return (0);
    }

    char path[0x100] = {0};

    for (int i = 1; command[i]; i++) {
        
        append_chr(path, command[i], 0xFF);
        
        if (command[i+1])
            append_chr(path, " ", 0xFF);
    }
    
    if (chdir(path) < 0) {
        perror(path);
        return (1);
    }

    return (0);
}

uint8_t show_help(char **command)
{
    printf("commands\t\tdescriptions\n-----------\t\t------------\n");
    printf("cd <path>\t\tchange working directory\n");
    printf("exit     \t\texit vaultshell\n");
    return (0);
}

uint8_t exit_shell(char **command)
{
    printf("[+] Thanks for using Vaultshell Bye !!!\n");
    exit(0);
}
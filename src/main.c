#include "vaultshell.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handle_sig(int sig)
{
    signal(sig, SIG_IGN);
}

int main(void)
{
    char buffer[0x100] = {0};
    char *command[0x100] = {0};

    signal(SIGINT, handle_sig);

    do {
        if (prompt("\n > ", buffer, 0xFF))
            perror("prompt()");
        
        split_command(buffer, command, ";&\n");
        handle_command(command);


        clear_buffer(buffer, sizeof(buffer));
    } while (1);
    
    return (0);
}
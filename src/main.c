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
    char buffer[0x100] = {0}, path[0x100] = {0}, host[0x100] = {0}, *command[0x100] = {0};
    prompt_info_t p = {0};
    
    getcwd(path, sizeof(path));
    gethostname(host, sizeof(host));

    set_prompt_info(&p, getenv("USER"), host, path);

    signal(SIGINT, handle_sig);

    do {

        getcwd(path, sizeof(path));

        if (prompt(&p, buffer, 0xFF))
            perror("prompt()");
        
        split_command(buffer, command, ";&\n");
        handle_command(command);


        clear_buffer(buffer, sizeof(buffer));
    } while (1);
    
    return (0);
}
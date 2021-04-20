#include "vaultshell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

char *cmd_name[] = {"cd", "help", "exit"};
uint8_t (*builtins_f[])(char **) = {&cd, &show_help, &exit_shell};

uint32_t handle_command(char **command)
{
    char *argv[0x100] = {0};
    int last = 0;

    for (int i = 0; command[i]; i++) {
        
        if (is_pipe(command[i])) {
            last = execute_command_pipe(command[i]);
            continue;
        }

        if (is_redirection(command[i])) {
            last = execute_redirection(command[i]);
            continue;
        }

        split_command(command[i], argv, " ");
        last = launch_command(argv, sizeof(cmd_name)/sizeof(cmd_name[0]));
    }

    return (last);
}

uint32_t launch_command(char **command, uint32_t number_of_builtins)
{
    for (uint32_t i = 0; i < number_of_builtins; i++) {
        if (strcmp(cmd_name[i], command[0]) == 0)
            return (builtins_f[i](command));
    }
    
    return (execute_command(command));
}


uint32_t execute_command(char **command)
{
    pid_t id = 0, status = 0;

    id = fork();

    if (id == 0) {
        // Child process
        if (execvp(command[0], command) < 0)
            perror(command[0]);
        
        exit(0);
    } else {
        // Parent process
        do {
            waitpid(id, &status, WUNTRACED);

            if (WEXITSTATUS(status))
                return (WEXITSTATUS(status));
            
            if (WTERMSIG(status))
                return (WTERMSIG(status));

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return (0);
}

uint32_t execute_command_pipe(char *command)
{
    char *argv[0x100] = {0}, *cmd[0x100] = {0};
    int fd[2], c_fd = 0, pid = 0, status = 0;

    replace_chr(command, '"', ' ');
    split_command(command, argv, "|");
    
    for (int i = 0; argv[i]; i++) {
        split_command(argv[i], cmd, " ");

        pipe(fd);

        pid = fork();

        if (pid == 0) {
            // Child process

            close(fd[0]);

            dup2(c_fd, STDIN_FILENO);

            if (argv[i+1])
                dup2(fd[1], STDOUT_FILENO);
            

            if (execvp(cmd[0], cmd) < 0)
                perror(cmd[0]);
            
            exit(0);
        } else {
            // Parent process
            waitpid(pid, &status, WUNTRACED);
            close(fd[1]);
            c_fd = fd[0];
        }

    }

    return (WEXITSTATUS(status));
}

uint32_t execute_redirection(char *command)
{
    char *argv[0x100] = {0};
    char *origin = command, *cmd = NULL, *out = NULL;
    int fd = 0, pid = 0, flag = O_WRONLY | O_CREAT;

    replace_chr(command, '"', ' ');

    if (is_redirection_append(command)) {
        flag = O_WRONLY | O_APPEND;
    }

    while (*command) {

        if (*command == '>') {
            
            if (command[1] == '>')
                command++;
            
            *command = 0;
            cmd = origin;
            out = ++command;
        }

        if (*command == '<') {
            
            if (command[1] == '<')
                command++;
            
            *command = 0;
            out = origin;
            cmd = ++command;
        }

        command++;
    }

    split_command(cmd, argv, " <>");

    erase_str(out, ' ');

    fd = open(out, flag, S_IRWXU);

    if (fd < 0) {
        perror("open");
        return (1);
    }
    
    pid = fork();

    if (pid == 0) {
        dup2(fd, STDOUT_FILENO);

        if (execvp(argv[0], argv) < 0)
            perror(argv[0]);
        
        exit(0);
    }

    wait(NULL);

    close(fd);

    return (0);
}

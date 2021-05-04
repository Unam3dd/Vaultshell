#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct prompt_info
{
    char *user;
    char *host;
    char *path;
} prompt_info_t;

// console
uint8_t prompt(prompt_info_t *prompt, char *buffer, size_t size);
uint8_t set_prompt_info(prompt_info_t *prompt, char *username, char *host, char *path);
void clear_buffer(char *buffer, size_t size);

// builtins
uint8_t cd(char **command);
uint8_t show_help(char **command);
uint8_t exit_shell(char **command);

// shell
uint32_t handle_command(char **command);
uint32_t launch_command(char **command, uint32_t number_of_builtins);
uint32_t execute_command(char **command);
uint32_t execute_command_pipe(char *command);
uint32_t execute_redirection(char *command);

// parsing
uint32_t split_command(char *buffer, char **command, char *delim);
char *replace_chr(char *buffer, char old, char new);
char *append_chr(char *buffer, char *src, size_t size_buffer);
uint32_t is_pipe(char *buffer);
uint32_t is_redirection(char *buffer);
uint8_t is_redirection_append(char *buffer);
char *erase_str(char *str, char chr, size_t len);
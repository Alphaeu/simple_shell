#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define END_OF_FILE -2
#define EXIT -3

/* Global environemnt */
extern char **environ;
/* Global program name */
char *name;
/* Global history counter */
int hist;

extern char **environ; /* Global environemnt */


/**
 * struct builtin_s - A new struct type to interpret builtin commands.
 * @name: The builtin command to name.
 * @f: A function pointer to the builtin command.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(char **argv, char **front);
} builtin_t;

/**
 * struct list_s - Defines a linked list.
 *@next: A ptr to another struct list_s.
 * @dir: Directory path.
 *
 */
typedef struct list_s
{
	char *dir;
	struct list_s *next;
} list_t;


/**
 * struct alias_s - A new struct to define aliases.
 * @name: the alias.
 * @value: Value of the alias.
 * @next: A ptr to next struct alias_s.
 */
typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

/* Global aliases linked list */
alias_t *aliases;

/* Main Shell Helpers */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
list_t *get_path_dir(char *path);
char **_strtok(char *line, char *delim);
char *get_location(char *command);
char *_itoa(int num);
int execute(char **args, char **front);
void free_list(list_t *head);

/* String_functions */
char *_strchr(char *s, char c);
int _strlen(const char *s);
char *_strcat(char *dest, const char *src);
int _strspn(char *s, char *accept);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strncat(char *dest, const char *src, size_t n);
char *_strcpy(char *dest, const char *src);

/* Input_Handlers */
void variable_replacement(char **args, int *exe_ret);
void handle_line(char **line, ssize_t read);
int handle_args(int *exe_ret);
int check_args(char **args);
char *get_args(char *line, int *exe_ret);
int call_args(char **args, char **front, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
void free_args(char **args, char **front);
char **replace_aliases(char **args);


/* Builtins_utils */
int (*get_builtin(char *command))(char **args, char **front);
int shellby_unsetenv(char **args, char __attribute__((__unused__)) **front);
int shellby_cd(char **args, char __attribute__((__unused__)) **front);
int shellby_env(char **args, char __attribute__((__unused__)) **front);
int shellby_setenv(char **args, char __attribute__((__unused__)) **front);
int shellby_alias(char **args, char __attribute__((__unused__)) **front);
int shellby_help(char **args, char __attribute__((__unused__)) **front);
int shellby_exit(char **args, char **front);

/* Error_Handling */
int create_error(char **args, int err);
char *error_env(char **args);
char *error_1(char **args);
char *error_2_exit(char **args);
char *error_2_cd(char **args);
char *error_2_syntax(char **args);
char *error_126(char **args);
char *error_127(char **args);

/* Builtin_Helpers */
char **_copyenv(void);
void free_env(void);
char **_getenv(const char *var);

/* Linkedlist Helpers */
alias_t *add_alias_end(alias_t **head, char *name, char *value);
void free_alias_list(alias_t *head);
list_t *add_node_end(list_t **head, char *dir);
void free_list(list_t *head);
void help_all(void);
void help_alias(void);
void help_cd(void);
void help_exit(void);
void help_help(void);
void help_env(void);
void help_setenv(void);
void help_unsetenv(void);
void help_history(void);

int proc_file_commands(char *file_path, int *exe_ret);

#endif

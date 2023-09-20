/*
 * File: helper.c
 * Auth: Alphaeus
 */

#include "shell.h"

void free_args(char **args, char **front);
char *get_pid(void);
char *get_env_value(char *beginning, int len);
void variable_replacement(char **args, int *exe_ret);

/**
 * free_args - frees up memory taken by args.
 * @args: A NULL-terminated double pointer containing commands.
 * @front: Double pointer to the beginning of args.
 */
void free_args(char **args, char **front)
{
	size_t x;

	for (x = 0; args[x] || args[x + 1]; x++)
		free(args[x]);

	free(front);
}

/**
 * get_pid - gets the current PID.
 * Description: opens the stat file, a space-delimitor file containing
 *              info about the current process. The function reads the PID into
 *              a buffer and replace the space at the end with a \0 byte.
 *
 * Return: The current PID or NULL on failure.
 */
char *get_pid(void)
{
	size_t x = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[x] != ' ')
		x++;
	buffer[x] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - gets the value to corresponding environmental var.
 * @beginning: the environmental var to search for.
 * @len: length of the environmental var to search for.
 *
 * Return: empty string if var not found.
 *         Otherwise - the value of the environmental variable.
 *
 * Description: Vars are stored in the format VARIABLE=VALUE.
 */
char *get_env_value(char *beginning, int len)
{
	char **var_addr;
	char *replacement = NULL, *temp, *var;

	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	var_addr = _getenv(var);
	free(var);
	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(_strlen(temp) + 1);
		if (replacement)
			_strcpy(replacement, temp);
	}

	return (replacement);
}

/**
 * variable_replacement - performs variable replacement.
 * @line: Double pointer containing the command and arguments.
 * @exe_ret: A ptr to the return value of the last executed command.
 *
 *
 * Description: Replaces $$ with the current PID, $? with the return value
 *              of the last executed program, and envrionmental variables
 *              preceded by $ with their corresponding value.
 */
void variable_replacement(char **line, int *exe_ret)
{
	int y, k = 0, len;
	char *replacement = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (y = 0; old_line[y]; y++)
	{
		if (old_line[y] == '$' && old_line[y + 1] &&
				old_line[y + 1] != ' ')
		{
			if (old_line[y + 1] == '$')
			{
				replacement = get_pid();
				k = y + 2;
			}
			else if (old_line[y + 1] == '?')
			{
				replacement = _itoa(*exe_ret);
				k = y + 2;
			}
			else if (old_line[y + 1])
			{
				/* Extract the variable name to search for */
				for (k = y + 1; old_line[k] &&
						old_line[k] != '$' &&
						old_line[k] != ' '; k++)
					;
				len = k - (y + 1);
				replacement = get_env_value(&old_line[y + 1], len);
			}
			new_line = malloc(y + _strlen(replacement)
					  + _strlen(&old_line[k]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, y);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[k]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			y = -1;
		}
	}
}

/*
 * File: input_helpers.c
 * Author: Alphaeus
 */

#include "shell.h"

char *get_args(char *line, int *exe_ret);
int call_args(char **args, char **front, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int check_args(char **args);

/**
 * get_args - gets a cmd from stdio.
 * @line: Buffer to store the command.
 * @exe_ret: Return value of the last executed command.
 *
 * Return: NULL on error.
 *         Otherwise - a ptr to the stored command.
 */
char *get_args(char *line, int *exe_ret)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	handle_line(&line, read);

	return (line);
}
/**
 * call_args - separates operators from commands & calls them.
 * @args: Array of args.
 * @front: double pointer to the beginning of args.
 * @exe_ret: value of the parent process' last executed command.
 *
 * Return: Value of the last executed command.
 */
int call_args(char **args, char **front, int *exe_ret)
{
	int ret, index;

	if (!args[0])
		return (*exe_ret);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}
	args = replace_aliases(args);
	ret = run_args(args, front, exe_ret);
	return (ret);
}

/**
 * run_args - initializes the execution of a command.
 * @args: Array of arguments.
 * @front: double pointer to the beginning of args.
 * @exe_ret: Return value of the parent process' last executed command.
 *
 * Return: Value of the last executed command.
 */
int run_args(char **args, char **front, int *exe_ret)
{
	int ret, x;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*exe_ret = ret;
	}
	else
	{
		*exe_ret = execute(args, front);
		ret = *exe_ret;
	}

	hist++;

	for (x = 0; args[x]; x++)
		free(args[x]);

	return (ret);
}

/**
 * handle_args - Finds, calls, and runs the command execution.
 * @exe_ret: Return value of the parent process' last executed command.
 *
 * Return:  END_OF_FILE (-2) -If an end-of-file is read.
 *         -1 -If the input cannot be tokenized.
 *         Otherwise - Exit value of the last executed command.
 */
int handle_args(int *exe_ret)
{
	int ret = 0, index;
	char **args, *line = NULL, **front;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (ret);
	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = call_args(args, front, exe_ret);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		ret = call_args(args, front, exe_ret);

	free(front);
	return (ret);
}

/**
 * check_args - Checks for any leading ';', ';;', '&&', or '||'.
 * @args: 2D ptr to tokenized cmd and args.
 *
 * Return: 2 -If a ';', '&&', or '||' is wrongly positioned.
 *	   Otherwise - 0.
 */
int check_args(char **args)
{
	size_t x;
	char *cur, *nex;

	for (x = 0; args[x]; x++)
	{
		cur = args[x];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (x == 0 || cur[1] == ';')
				return (create_error(&args[x], 2));
			nex = args[x + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (create_error(&args[x + 1], 2));
		}
	}
	return (0);
}

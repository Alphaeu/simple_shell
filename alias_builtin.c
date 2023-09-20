/*
 * File: alias_builtin.c
 * Author: Alphaeus
 */

#include "shell.h"

int shellby_alias(char **args, char __attribute__((__unused__)) **front);
void set_alias(char *var_name, char *value);
void print_alias(alias_t *alias);

/**
 * shellby_alias - Command to either prints all aliases, specific
 *                 aliases, or sets an alias.
 * @args: Array of args.
 * @front: A double pointer to the beginning of arguements.
 *
 * Return: -1 on error.
 *         Otherwise - 0.
 */
int shellby_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	int x, ret = 0;
	char *value;

	if (!args[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return (ret);
	}
	for (x = 0; args[x]; x++)
	{
		temp = aliases;
		value = _strchr(args[x], '=');
		if (!value)
		{
			while (temp)
			{
				if (_strcmp(args[x], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				ret = create_error(args + x, 1);
		}
		else
			set_alias(args[x], value);
	}
	return (ret);
}

/**
 * set_alias - Sets either an existing alias 'name' with a new value,
 *            'value' or creates a new alias with 'name' and 'value'.
 * @var_name: Alias name.
 * @value: Value of the alias with first character as '='.
 */
void set_alias(char *var_name, char *value)
{
	alias_t *temp = aliases;
	int len, y, k;
	char *new_value;

	*value = '\0';
	value++;
	len = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return;
	for (y = 0, k = 0; value[y]; y++)
	{
		if (value[y] != '\'' && value[y] != '"')
			new_value[k++] = value[y];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		add_alias_end(&aliases, var_name, new_value);
}

/**
 * print_alias - prints the alias in the format name='value'.
 * @alias: alias pointer
 */
void print_alias(alias_t *alias)
{
	char *alias_string;
	int len = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (len + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, len);
	free(alias_string);
}
/**
 * replace_aliases - replaces any matching alias in the arguements
 *                   with their value.
 * @args: 2D pointer to the ags.
 *
 * Return: 2D pointer to the args.
 */
char **replace_aliases(char **args)
{
	alias_t *temp;
	int x;
	char *new_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);
	for (x = 0; args[x]; x++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(args[x], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (_strlen(temp->value) + 1));
				if (!new_value)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(new_value, temp->value);
				free(args[x]);
				args[x] = new_value;
				x--;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);
}

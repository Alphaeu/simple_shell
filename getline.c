#include "shell.h"

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);

/**
 * _realloc - reallocates a memory block using malloc and free.
 * @ptr: A ptr to previously allocated memory.
 * @old_size: Size in bytes of the allocated space for ptr.
 * @new_size: Size in bytes for the new memory block.
 *
 *
 * Return: If new_size == old_size - ptr.
 *         If new_size == 0 and ptr is not NULL - NULL.
 *         Otherwise - a pointer to the reallocated memory block.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *mem;
	char *ptr_copy, *filler;
	unsigned int index;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		mem = malloc(new_size);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_copy = ptr;
	mem = malloc(sizeof(*ptr_copy) * new_size);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mem;

	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mem);
}

/**
 * assign_lineptr - Re-assigns the lineptr variable for _getline.
 * @lineptr: Buffer to store an input string.
 * @n: size of lineptr.
 * @buffer: the string to assign to lineptr.
 * @b: Size of buffer.
 */
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t a)
{
	if (*lineptr == NULL)
	{
		if (a > 120)
			*n = a;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else if (*n < a)
	{
		if (a > 120)
			*n = a;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * _getline - checks input from a stream.
 * @lineptr: Buffer to store the input.
 * @n: Size of lineptr.
 * @stream: Stream to read from.
 *
 * Return: The Number of bytes read.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int t;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		t = read(STDIN_FILENO, &c, 1);
		if (t == -1 || (t == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (t == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = _realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	assign_lineptr(lineptr, n, buffer, input);

	ret = input;
	if (t != 0)
		input = 0;
	return (ret);
}

/*
 * File: str_funcs1.c
 * Author: Alphaeus
 */

#include "shell.h"

int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);

/**
 * _strlen - length of a string.
 * @s: A ptr to the characters in a string.
 *
 * Return: Length of the character in a string.
 */
int _strlen(const char *s)
{
	int length = 0;

	if (!s)
		return (length);
	for (length = 0; s[length]; length++)
		;
	return (length);
}

/**
 * _strcpy - To copy the string pointed to by src, & terminating
 *           the null byte of the buffer pointed by des.
 * @dest: Ptr to the destination of copied string.
 * @src: Ptr to the src of the source string.
 *
 * Return: Pointer to dest.
 */
char *_strcpy(char *dest, const char *src)
{
	size_t i;

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

/**
 * _strcat - concantenates 2 strings.
 * @dest: Ptr to destination string.
 * @src: Ptr to source string.
 *
 * Return: Ptr to destination string.
 */
char *_strcat(char *dest, const char *src)
{
	char *destTemp;
	const char *srcTemp;

	destTemp = dest;
	srcTemp =  src;

	while (*destTemp != '\0')
		destTemp++;

	while (*srcTemp != '\0')
		*destTemp++ = *srcTemp++;
	*destTemp = '\0';
	return (dest);
}

/**
 * _strncat - concantenates 2 str where n number of bytes
 *             are copied from source.
 * @dest: Ptr to destination string.
 * @src: Ptr to source string.
 * @n: n bytes to copy from src.
 * Return: Ptr to destination string.
 */
char *_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = _strlen(dest);
	size_t x;

	for (x = 0; x < n && src[x] != '\0'; x++)
		dest[dest_len + x] = src[x];
	dest[dest_len + x] = '\0';

	return (dest);
}

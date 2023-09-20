/*
 * File: str_funcs2.c
 * Author: Alphaeus
 */

#include "shell.h"

char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);

/**
 * _strchr - locates a char in a str.
 * @s: str to be searched.
 * @c: Char to be located.
 * Return: If c is found - a ptr to the first occurence.
 *         If c is not found - NULL.
 */
char *_strchr(char *s, char c)
{
	int index;

	for (index = 0; s[index]; index++)
	{
		if (s[index] == c)
			return (s + index);
	}

	return (NULL);
}

/**
 * _strspn - the length of a prefix substring.
 * @s: Str to be searched.
 * @accept: Prefix to be measured.
 *
 * Return: Number of bytes in s which consist
 *          only of bytes from accept.
 */
int _strspn(char *s, char *accept)
{
	int bytes = 0;
	int index;

	while (*s)
	{
		for (index = 0; accept[index]; index++)
		{
			if (*s == accept[index])
			{
				bytes++;
				break;
			}
		}
		s++;
	}
	return (bytes);
}

/**
 * _strcmp - To compare two strings.
 * @s1: First string to be compared.
 * @s2: Second string to be compared.
 *
 * Return: +ve byte difference if s1 > s2
 *         0 if s1 = s2
 *         -ve byte difference if s1 < s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	if (*s1 != *s2)
		return (*s1 - *s2);

	return (0);
}

/**
 * _strncmp - To compare two strings.
 * @s1: Ptr to a string.
 * @s2: Ptr to a string.
 * @n: First n bytes of the strings to compare.
 *
 * Return: < 0 if s1 is shorter than s2.
 *         0 if s1 == s2.
 *         > 0 if s1 is longer than s2.
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t x;

	for (x = 0; s1[x] && s2[x] && x < n; x++)
	{
		if (s1[x] > s2[x])
			return (s1[x] - s2[x]);
		else if (s1[x] < s2[x])
			return (s1[x] - s2[x]);
	}
	if (x == n)
		return (0);
	else
		return (-15);
}

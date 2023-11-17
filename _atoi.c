#include "boshell.h"

/**
 * interactive - if the shell is in interactive mode, returns true.
 * @binfo: address of struct
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive(binfo_t *binfo)
{
	return (isatty(STDIN_FILENO) && binfo->readfd <= 2);
}

/**
 * bdelim - determines whether a character is a delimeter
 * @c: the char to examine
 * @delim: the string delimeter
 * Return: 1 if true, 0 if false
 */
int bdelim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 *_isalpha - looks for characters in the alphabet
 *@c: The input character
 *Return: 1 if c is alphabetic, 0 otherwise
 */

int _isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *_atoi - creates an integer from a string
 *@s: the text that has to be transformed
 *Return: 0 if no numbers in string, converted number otherwise
 */

int _atoi(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0;  s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

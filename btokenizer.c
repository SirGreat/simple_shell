#include "boshell.h"

/**
 * **strtow - divides a string alphabetically. Repeat separators are not used
 * @bstr: the input string
 * @d: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **strtow(char *bstr, char *d)
{
	int i, j, k, m, numwords = 0;
	char **s;

	if (bstr == NULL || bstr[0] == 0)
		return (NULL);
	if (!d)
		d = " ";
	for (i = 0; bstr[i] != '\0'; i++)
		if (!bdelim(bstr[i], d) && (bdelim(bstr[i + 1], d) || !bstr[i + 1]))
			numwords++;

	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (bdelim(bstr[i], d))
			i++;
		k = 0;
		while (!bdelim(bstr[i + k], d) && bstr[i + k])
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = bstr[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;
	return (s);
}

/**
 * **strtow2 - splits a string into words
 * @bstr: the input string
 * @d: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **strtow2(char *bstr, char d)
{
	int i, j, k, m, numwords = 0;
	char **s;

	if (bstr == NULL || bstr[0] == 0)
		return (NULL);
	for (i = 0; bstr[i] != '\0'; i++)
		if ((bstr[i] != d && bstr[i + 1] == d) ||
		    (bstr[i] != d && !bstr[i + 1]) || bstr[i + 1] == d)
			numwords++;
	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (bstr[i] == d && bstr[i] != d)
			i++;
		k = 0;
		while (bstr[i + k] != d && bstr[i + k] && bstr[i + k] != d)
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = bstr[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;
	return (s);
}

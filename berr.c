#include "boshell.h"

/**
 *_bputs - prints an input string
 * @bstr: the string to be printed
 *
 * Return: Nothing
 */
void _bputs(char *bstr)
{
	int i = 0;

	if (!bstr)
		return;
	while (bstr[i] != '\0')
	{
		_bputchar(bstr[i]);
		i++;
	}
}

/**
 * _bputchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _bputchar(char c)
{
	static int i;
	static char buf[BWBUF_SIZE];

	if (c == BBUF_FLUSH || i >= BWBUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BBUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _bputfd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _bputfd(char c, int fd)
{
	static int i;
	static char buf[BWBUF_SIZE];

	if (c == BBUF_FLUSH || i >= BWBUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BBUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 *_bputsfd - prints an input string
 * @bstr: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _bputsfd(char *bstr, int fd)
{
	int i = 0;

	if (!bstr)
		return (0);
	while (*bstr)
	{
		i += _bputfd(*bstr++, fd);
	}
	return (i);
}

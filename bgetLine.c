#include "boshell.h"

/**
 * input_buf - buffers chained commands
 * @binfo: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(binfo_t *binfo, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)binfo->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _bgetline(binfo, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			binfo->linecount_flag = 1;
			remove_comments(*buf);
			build_bhistory_list(binfo, *buf, binfo->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				binfo->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line minus the newline
 * @binfo: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(binfo_t *binfo)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(binfo->arg), *b;

	_putchar(BBUF_FLUSH);
	r = input_buf(binfo, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		b = buf + i; /* get pointer for return */

		check_bchain(binfo, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (bchain(binfo, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			binfo->cmd_buf_type = BMD_NORM;
		}

		*buf_p = b; /* pass back pointer to current command position */
		return (_strlen(b)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _bgetline() */
	return (r); /* return length of buffer from _bgetline() */
}

/**
 * read_buf - reads a buffer
 * @binfo: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buf(binfo_t *binfo, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(binfo->readfd, buf, BBUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _bgetline - gets the next line of input from STDIN
 * @binfo: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _bgetline(binfo_t *binfo, char **ptr, size_t *length)
{
	static char buf[BBUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *b = NULL, *new_p = NULL, *c;

	b = *ptr;
	if (b && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(binfo, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(b, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (b ? free(b), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	b = new_p;

	if (length)
		*length = s;
	*ptr = b;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BBUF_FLUSH);
}

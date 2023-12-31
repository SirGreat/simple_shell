#include "boshell.h"

/**
 * bchain - verify if the buffer's current character is a chain delimeter.
 * @binfo: the structural parameter
 * @buf: the buffer for char
 * @b: current position's address in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int bchain(binfo_t *binfo, char *buf, size_t *b)
{
	size_t j = *b;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		binfo->cmd_buf_type = BMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		binfo->cmd_buf_type = BMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		binfo->cmd_buf_type = BMD_CHAIN;
	}
	else
		return (0);
	*b = j;
	return (1);
}

/**
 * check_bchain - checks we should continue chaining based on last status
 * @binfo: the structural parameter
 * @buf: the buffer for char
 * @b: current position's address in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_bchain(binfo_t *binfo, char *buf, size_t *b, size_t i, size_t len)
{
	size_t j = *b;

	if (binfo->cmd_buf_type == BMD_AND)
	{
		if (binfo->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (binfo->cmd_buf_type == BMD_OR)
	{
		if (!binfo->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*b = j;
}

/**
 * replace_balias - replaces an aliases in the tokenized string
 * @binfo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_balias(binfo_t *binfo)
{
	int i;
	list_t *node;
	char *b;

	for (i = 0; i < 10; i++)
	{
		node = bnode_starts_with(binfo->balias, binfo->argv[0], '=');
		if (!node)
			return (0);
		free(binfo->argv[0]);
		b = _strchr(node->bstr, '=');
		if (!b)
			return (0);
		b = _strdup(b + 1);
		if (!b)
			return (0);
		binfo->argv[0] = b;
	}
	return (1);
}

/**
 * replace_bvars - replaces vars in the tokenized string
 * @binfo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_bvars(binfo_t *binfo)
{
	int i = 0;
	list_t *node;

	for (i = 0; binfo->argv[i]; i++)
	{
		if (binfo->argv[i][0] != '$' || !binfo->argv[i][1])
			continue;

		if (!_strcmp(binfo->argv[i], "$?"))
		{
			replace_bstring(&(binfo->argv[i]),
				_strdup(convert_number(binfo->status, 10, 0)));
			continue;
		}
		if (!_strcmp(binfo->argv[i], "$$"))
		{
			replace_bstring(&(binfo->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = bnode_starts_with(binfo->env, &binfo->argv[i][1], '=');
		if (node)
		{
			replace_bstring(&(binfo->argv[i]),
				_strdup(_strchr(node->bstr, '=') + 1));
			continue;
		}
		replace_bstring(&binfo->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_bstring - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_bstring(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

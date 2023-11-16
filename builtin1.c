#include "boshell.h"

/**
 * _bhistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @binfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _bhistory(binfo_t *binfo)
{
	bprint_list(binfo->history);
	return (0);
}

/**
 * unset_balias - sets balias to string
 * @binfo: parameter struct
 * @bstr: the string balias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_balias(binfo_t *binfo, char *bstr)
{
	char *b, c;
	int bret;

	b = _strchr(bstr, '=');
	if (!b)
		return (1);
	c = *b;
	*b = 0;
	bret = bdelete_node_at_index(&(binfo->balias),
	get_node_index(binfo->balias, bnode_starts_with(binfo->balias, bstr, -1)));
	*b = c;
	return (bret);
}

/**
 * set_balias - sets balias to string
 * @binfo: parameter struct
 * @bstr: the string balias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_balias(binfo_t *binfo, char *bstr)
{
	char *b;

	b = _strchr(bstr, '=');
	if (!b)
		return (1);
	if (!*++b)
		return (unset_balias(binfo, bstr));

	unset_balias(binfo, bstr);
	return (badd_node_end(&(binfo->balias), bstr, 0) == NULL);
}

/**
 * print_balias - prints an balias string
 * @node: the balias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_balias(list_t *node)
{
	char *b = NULL, *a = NULL;

	if (node)
	{
		b = _strchr(node->bstr, '=');
		for (a = node->bstr; a <= b; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(b + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _mybalias - mimics the alias builtin (man alias)
 * @binfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _mybalias(binfo_t *binfo)
{
	int i = 0;
	char *b = NULL;
	list_t *node = NULL;

	if (binfo->argc == 1)
	{
		node = binfo->balias;
		while (node)
		{
			print_balias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; binfo->argv[i]; i++)
	{
		b = _strchr(binfo->argv[i], '=');
		if (b)
			set_balias(binfo, binfo->argv[i]);
		else
			print_balias(bnode_starts_with(binfo->balias, binfo->argv[i], '='));
	}

	return (0);
}

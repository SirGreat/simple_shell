#include "boshell.h"

/**
 * _benv - prints the current environment
 * @binfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _benv(binfo_t *binfo)
{
	bprint_list_str(binfo->env);
	return (0);
}

/**
 * _bgetenv - gets the value of an environ variable
 * @binfo: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_bgetenv(binfo_t *binfo, const char *name)
{
	list_t *node = binfo->env;
	char *b;

	while (node)
	{
		b = bstarts_with(node->bstr, name);
		if (b && *b)
			return (b);
		node = node->next;
	}
	return (NULL);
}

/**
 * _bmsetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @binfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _bmsetenv(binfo_t *binfo)
{
	if (binfo->argc != 3)
	{
		_bputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_bsetenv(binfo, binfo->argv[1], binfo->argv[2]))
		return (0);
	return (1);
}

/**
 * _bunsetenv - Remove an environment variable
 * @binfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _bunsetenv(binfo_t *binfo)
{
	int i;

	if (binfo->argc == 1)
	{
		_bputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= binfo->argc; i++)
		b_unsetenv(binfo, binfo->argv[i]);

	return (0);
}

/**
 * bpopulate_env_list - populates env linked list
 * @binfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int bpopulate_env_list(binfo_t *binfo)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		badd_node_end(&node, environ[i], 0);
	binfo->env = node;
	return (0);
}

#include "boshell.h"

/**
 * bget_env - returns the string array copy of our environ
 * @binfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **bget_env(binfo_t *binfo)
{
	if (!binfo->environ || binfo->env_changed)
	{
		binfo->environ = blist_to_strings(binfo->env);
		binfo->env_changed = 0;
	}

	return (binfo->environ);
}

/**
 * b_unsetenv - Remove an environment variable
 * @binfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int b_unsetenv(binfo_t *binfo, char *var)
{
	list_t *node = binfo->env;
	size_t i = 0;
	char *b;

	if (!node || !var)
		return (0);

	while (node)
	{
		 = bstarts_with(node->bstr, var);
		if (b && *b == '=')
		{
			binfo->env_changed = bdelete_node_at_index(&(binfo->env), i);
			i = 0;
			node = binfo->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (binfo->env_changed);
}

/**
 * _bsetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @binfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _bsetenv(binfo_t *binfo, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *b;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = binfo->env;
	while (node)
	{
		b = bstarts_with(node->bstr, var);
		if (b && *b == '=')
		{
			free(node->str);
			node->str = buf;
			binfo->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	badd_node_end(&(binfo->env), buf, 0);
	free(buf);
	binfo->env_changed = 1;
	return (0);
}

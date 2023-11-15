#include "boshell.h"

/**
 * clear_binfo - initializes binfo_t struct
 * @binfo: struct address
 */
void clear_binfo(binfo_t *binfo)
{
	binfo->arg = NULL;
	binfo->argv = NULL;
	binfo->path = NULL;
	binfo->argc = 0;
}

/**
 * set_binfo - initializes binfo_t struct
 * @binfo: struct address
 * @av: argument vector
 */
void set_binfo(binfo_t *binfo, char **av)
{
	int i = 0;

	binfo->fname = av[0];
	if (binfo->arg)
	{
		binfo->argv = strtow(binfo->arg, " \t");
		if (!binfo->argv)
		{

			binfo->argv = malloc(sizeof(char *) * 2);
			if (binfo->argv)
			{
				binfo->argv[0] = _strdup(binfo->arg);
				binfo->argv[1] = NULL;
			}
		}
		for (i = 0; binfo->argv && binfo->argv[i]; i++)
			;
		binfo->argc = i;

		replace_balias(binfo);
		replace_bvars(binfo);
	}
}

/**
 * free_binfo - frees binfo_t struct fields
 * @binfo: struct address
 * @all: true if freeing all fields
 */
void free_binfo(binfo_t *binfo, int all)
{
	ffree(binfo->argv);
	binfo->argv = NULL;
	binfo->path = NULL;
	if (all)
	{
		if (!binfo->cmd_buf)
			free(binfo->arg);
		if (binfo->env)
			free_blist(&(binfo->env));
		if (binfo->history)
			free_blist(&(binfo->history));
		if (binfo->balias)
			free_blist(&(binfo->balias));
		ffree(binfo->environ);
			binfo->environ = NULL;
		bfree((void **)binfo->cmd_buf);
		if (binfo->readfd > 2)
			close(binfo->readfd);
		_putchar(BBUF_FLUSH);
	}
}

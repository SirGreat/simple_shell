#include "boshell.h"

/**
 * hsh - main shell loop
 * @binfo: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(binfo_t *binfo, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_binfo(binfo);
		if (interactive(binfo))
			_puts("$ ");
		_bputchar(BBUF_FLUSH);
		r = get_input(binfo);
		if (r != -1)
		{
			set_binfo(binfo, av);
			builtin_ret = find_builtin(binfo);
			if (builtin_ret == -1)
				find_cmd(binfo);
		}
		else if (interactive(binfo))
			_putchar('\n');
		free_binfo(binfo, 0);
	}
	write_bhistory(binfo);
	free_binfo(binfo, 1);
	if (!interactive(binfo) && binfo->status)
		exit(binfo->status);
	if (builtin_ret == -2)
	{
		if (binfo->err_num == -1)
			exit(binfo->status);
		exit(binfo->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @binfo: the parameter & return binfo struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(binfo_t *binfo)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _bexit},
		{"env", _benv},
		{"help", _bhelp},
		{"history", _bhistory},
		{"setenv", _bmsetenv},
		{"unsetenv", _bunsetenv},
		{"cd", _bcd},
		{"alias", _mybalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(binfo->argv[0], builtintbl[i].type) == 0)
		{
			binfo->line_count++;
			built_in_ret = builtintbl[i].func(binfo);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @binfo: the parameter & return binfo struct
 *
 * Return: void
 */
void find_cmd(binfo_t *binfo)
{
	char *path = NULL;
	int i, k;

	binfo->path = binfo->argv[0];
	if (binfo->linecount_flag == 1)
	{
		binfo->line_count++;
		binfo->linecount_flag = 0;
	}
	for (i = 0, k = 0; binfo->arg[i]; i++)
		if (!bdelim(binfo->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_bpath(binfo, _bgetenv(binfo, "PATH="), binfo->argv[0]);
	if (path)
	{
		binfo->path = path;
		fork_cmd(binfo);
	}
	else
	{
		if ((interactive(binfo) || _bgetenv(binfo, "PATH=")
			|| binfo->argv[0][0] == '/') && bcmd(binfo, binfo->argv[0]))
			fork_cmd(binfo);
		else if (*(binfo->arg) != '\n')
		{
			binfo->status = 127;
			print_error(binfo, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @binfo: the parameter & return binfo struct
 *
 * Return: void
 */
void fork_cmd(binfo_t *binfo)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(binfo->path, binfo->argv, bget_env(binfo)) == -1)
		{
			free_binfo(binfo, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(binfo->status));
		if (WIFEXITED(binfo->status))
		{
			binfo->status = WEXITSTATUS(binfo->status);
			if (binfo->status == 126)
				print_error(binfo, "Permission denied\n");
		}
	}
}

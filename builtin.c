#include "boshell.h"

/**
 * _bexit - exits the shell
 * @binfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int _bexit(binfo_t *binfo)
{
	int bexitcheck;

	if (binfo->argv[1])  /* If there is an exit arguement */
	{
		bexitcheck = _berratoi(binfo->argv[1]);
		if (bexitcheck == -1)
		{
			binfo->status = 2;
			print_error(binfo, "Illegal number: ");
			_bputs(binfo->argv[1]);
			_bputchar('\n');
			return (1);
		}
		binfo->err_num = _berratoi(binfo->argv[1]);
		return (-2);
	}
	binfo->err_num = -1;
	return (-2);
}

/**
 * _bcd - changes the current directory of the process
 * @binfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _bcd(binfo_t *binfo)
{
	char *s, *bdir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!binfo->argv[1])
	{
		bdir = _bgetenv(binfo, "HOME=");
		if (!bdir)
			chdir_ret = /* TODO: what should this be? */
				chdir((bdir = _bgetenv(binfo, "PWD=")) ? bdir : "/");
		else
			chdir_ret = chdir(bdir);
	}
	else if (_strcmp(binfo->argv[1], "-") == 0)
	{
		if (!_bgetenv(binfo, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_bgetenv(binfo, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((bdir = _bgetenv(binfo, "OLDPWD=")) ? bdir : "/");
	}
	else
		chdir_ret = chdir(binfo->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(binfo, "can't cd to ");
		_bputs(binfo->argv[1]), _bputchar('\n');
	}
	else
	{
		_bsetenv(binfo, "OLDPWD", _bgetenv(binfo, "PWD="));
		_bsetenv(binfo, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _bhelp - changes the current directory of the process
 * @binfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _bhelp(binfo_t *binfo)
{
	char **arg_array;

	arg_array = binfo->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}

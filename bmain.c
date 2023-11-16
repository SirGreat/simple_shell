#include "boshell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	binfo_t binfo[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_bputs(av[0]);
				_bputs(": 0: Can't open ");
				_bputs(av[1]);
				_bputchar('\n');
				_bputchar(BBUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		binfo->readfd = fd;
	}
	bpopulate_env_list(binfo);
	read_bhistory(binfo);
	hsh(binfo, av);
	return (EXIT_SUCCESS);
}

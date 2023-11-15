#include "boshell.h"

/**
 * get_bhistory_file - gets the history file
 * @binfo: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_bhistory_file(binfo_t *binfo)
{
	char *buf, *bdir;

	dir = _bgetenv(binfo, "HOME=");
	if (!bdir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(bdir) + _strlen(BHIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, bdir);
	_strcat(buf, "/");
	_strcat(buf, BHIST_FILE);
	return (buf);
}

/**
 * write_bhistory - creates a file, or appends to an existing file
 * @binfo: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_bhistory(binfo_t *binfo)
{
	ssize_t fd;
	char *filename = get_bhistory_file(binfo);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = binfo->history; node; node = node->next)
	{
		_bputsfd(node->bstr, fd);
		_bputfd('\n', fd);
	}
	_bputfd(BBUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_bhistory - reads history from file
 * @binfo: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_bhistory(binfo_t *binfo)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_bhistory_file(binfo);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_bhistory_list(binfo, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_bhistory_list(binfo, buf + last, linecount++);
	free(buf);
	binfo->histcount = linecount;
	while (binfo->histcount-- >= BHIST_MAX)
		bdelete_node_at_index(&(binfo->history), 0);
	renumber_history(binfo);
	return (binfo->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @binfo: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_history_list(binfo_t *binfo, char *buf, int linecount)
{
	list_t *node = NULL;

	if (binfo->history)
		node = binfo->history;
	badd_node_end(&node, buf, linecount);

	if (!binfo->history)
		binfo->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @binfo: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(binfo_t *binfo)
{
	list_t *node = binfo->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (binfo->histcount = i);
}

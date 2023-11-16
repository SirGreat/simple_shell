#ifndef BOSHELL_H
#define BOSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define BBUF_SIZE 1024
#define BWBUF_SIZE 1024
#define BBUF_FLUSH -1

/* for command chaining */
#define BMD_NORM	0
#define BMD_OR		1
#define BMD_AND		2
#define BMD_CHAIN	3

/* for convert_number() */
#define BCONVERT_LOWERCASE	1
#define BCONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define BHIST_FILE	".simple_shell_history"
#define BHIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @bstr: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *bstr;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@balias: the balias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *balias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} binfo_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(binfo_t *);
} builtin_table;


/* bshell_start.c */
int hsh(binfo_t *, char **);
int find_builtin(binfo_t *);
void find_cmd(binfo_t *);
void fork_cmd(binfo_t *);

/* bmem-parser.c */
int bcmd(binfo_t *, char *);
char *bdup_chars(char *, int, int);
char *find_bpath(binfo_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* berr.c */
void _bputs(char *);
int _bputchar(char);
int _bputfd(char c, int fd);
int _bputsfd(char *bstr, int fd);

/* bstring.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *bstarts_with(const char *, const char *);
char *_strcat(char *, char *);

/* bstring1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* bexits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* btokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* brealloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* memory.c */
int bfree(void **);

/* atoi.c */
int interactive(binfo_t *);
int bdelim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* berrors1.c */
int _berratoi(char *);
void print_error(binfo_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* builtin.c */
int _bexit(binfo_t *);
int _bcd(binfo_t *);
int _bhelp(binfo_t *);

/* builtin1.c */
int _bhistory(binfo_t *);
int _mybalias(binfo_t *);

/* bgetline.c */
ssize_t get_input(binfo_t *);
int _bgetline(binfo_t *, char **, size_t *);
void sigintHandler(int);

/* bgetinfo.c */
void clear_binfo(binfo_t *);
void set_binfo(binfo_t *, char **);
void free_binfo(binfo_t *, int);

/* benv.c */
char *_bgetenv(binfo_t *, const char *);
int _benv(binfo_t *);
int _bmsetenv(binfo_t *);
int _bunsetenv(binfo_t *);
int bpopulate_env_list(binfo_t *);

/* bgetenv.c */
char **bget_env(binfo_t *);
int b_unsetenv(binfo_t *, char *);
int _bsetenv(binfo_t *, char *, char *);

/* bhistory.c */
char *get_bhistory_file(binfo_t *binfo);
int write_bhistory(binfo_t *binfo);
int read_bhistory(binfo_t *binfo);
int build_bhistory_list(binfo_t *binfo, char *buf, int linecount);
int renumber_history(binfo_t *binfo);

/* blists.c */
list_t *badd_node(list_t **, const char *, int);
list_t *badd_node_end(list_t **, const char *, int);
size_t bprint_list_str(const list_t *);
int bdelete_node_at_index(list_t **, unsigned int);
void free_blist(list_t **);

/* blists1.c */
size_t blist_len(const list_t *);
char **blist_to_strings(list_t *);
size_t bprint_list(const list_t *);
list_t *bnode_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* bchain.c */
int bchain(binfo_t *, char *, size_t *);
void check_bchain(binfo_t *, char *, size_t *, size_t, size_t);
int replace_balias(binfo_t *);
int replace_bvars(binfo_t *);
int replace_bstring(char **, char *);

#endif /* BOSHELL_H */

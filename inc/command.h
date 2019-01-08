#ifndef COMMAND_H
# define COMMAND_H

#include "libft.h"

# define COMMANDS_LEN 11

typedef struct	s_arg
{
	char const	*name;
	char const	*desc;
}				t_arg;

typedef struct	s_command
{
	char const	*name;
	char const	*desc;
	t_arg const	*(args);
	size_t		nargs;
	t_bool		local;
	t_bool		(*exec)(char * const *args, int sock,
			char const **reason, t_bool *should_quit);
}				t_command;

t_bool		exec_cmd_help(char * const *args, int sock, char const **reason, t_bool *should_quit);

t_bool	command_exec(char * const *args, char const **reason, int sock, t_bool *should_quit);

t_bool	exec_cmd_ping(char * const *args, int sock, char const **reason,
		t_bool *should_quit);

#endif

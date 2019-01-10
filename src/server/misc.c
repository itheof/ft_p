#include "server.h"

#define LOG_PADDING 8

void	print_header(pid_t pid)
{
	pid_t	tmp;
	int		i;

	tmp = pid;
	i = sizeof("()") - sizeof("");
	while (tmp > 0)
	{
		tmp /= 10;
		i++;
	}
	assert(LOG_PADDING >= i);
	if (pid) // this can be optimized obviously
		printf("worker(%d)%*c: ", pid, LOG_PADDING - i, ' ');
	else
		printf("\033[31m" "master" "\033[0m" "%*c: ", LOG_PADDING, ' ');
}

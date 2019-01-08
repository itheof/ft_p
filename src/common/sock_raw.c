#include "message.h"

ssize_t		sock_raw_read(int fildes, void *buf, ssize_t nbyte)
{
	ssize_t	ret;
	ssize_t	count;

	count = 0;
	while (count != nbyte &&
			(ret = read(fildes, (char*)buf + count, nbyte - count)) > 0)
		count += ret;
	return ((ret < 0) ? ret : count);
}

ssize_t		sock_raw_write(int fildes, void *buf, ssize_t nbyte)
{
	ssize_t	ret;
	ssize_t	count;

	count = 0;
	while (count != nbyte &&
			(ret = write(fildes, (char*)buf + count, nbyte - count)) > 0)
		count += ret;
	return ((ret < 0) ? ret : count);
}

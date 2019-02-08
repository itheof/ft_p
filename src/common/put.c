#include "common.h"
#include <dirent.h>
#include <limits.h>

static const char	g_not_basename_err[] = "put: file must be a single path "
"component, relative to the current directory";

static const char	g_exists_err[] = "put: file exists";

//TODO: enhance to -> is_same_dir
static t_bool	is_basename(char const *path)
{
	if (!ft_strcmp(".", path))
		return (false);
	else if (!ft_strcmp("..", path))
		return (false);
	return (ft_strchr(path, '/') == NULL);
}

static t_bool	file_exists(char const *filename, t_env *e)
{
	struct dirent	*dp;
	t_bool			ret;
	DIR				*dirp;

	dirp = opendir(".");
	if (dirp == NULL)
	{
		e->log(e, "put: opendir() failed: %s", strerror(errno));
		return (false);
	}
	ret = true;
	while ((dp = readdir(dirp)) != NULL)
	{
		if (!strcmp(dp->d_name, filename))
		{
			ret = false;
			break;
		}
	}
	closedir(dirp);
	return (ret);
}

static t_ecode	sanitize_filename(char const *filename, t_env *e)
{
	t_ecode	err;

	if (!is_basename(filename))
	{
		if ((err = message_send(E_MESSAGE_ERR, g_not_basename_err,
					sizeof(g_not_basename_err), e->csock)))
			return (err);
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	if (!file_exists(filename, e))
	{
		if ((err = message_send(E_MESSAGE_ERR, g_exists_err,
					sizeof(g_exists_err), e->csock)))
			return (err);
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	return (E_ERR_OK);
}

static t_ecode	get_file_size(off_t *dsize, t_env *e)
{
	t_message	*msg;
	t_ecode		err;

	if ((err = message_receive(&msg, e->csock)))
	{
		e->should_quit = true;
		return (err);
	}
	if (msg->hd.size != sizeof(*dsize))
	{
		message_destroy(msg);
		e->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	*dsize = *((off_t*)msg->payload);
	message_destroy(msg);
	return (E_ERR_OK);
}

static t_ecode	prepare_transfer(char *path, off_t size, int *dfd, void **dmap)
{
	int	saved_errno;

	if ((*dfd = open(path, O_RDWR | O_CREAT | O_EXCL, 0644)) < 0)
		return (E_ERR_OPEN);
	if (ftruncate(*dfd, size) != 0)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_FTRUNC);
	}
	if ((*dmap = mmap(0, size, PROT_WRITE, MAP_SHARED, *dfd, 0)) == MAP_FAILED)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_MMAP);
	}
	return (E_ERR_OK);
}

static t_ecode	server_transfer(void *map, off_t size, t_env *e)
{
	if (sock_raw_read(e->csock, map, size) < 0)
	{
		e->log(e, "put: read(): %s", strerror(errno));
		e->should_quit = true;
		return (E_ERR_READ);
	}
	return (E_ERR_OK);
}

t_ecode	put_op_handler(t_message *msg, t_env *e)
{
	int 	fd;
	off_t	size;
	void	*map;
	t_ecode	err;

	if (!msg->hd.size)
	{
		e->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	msg->payload[msg->hd.size - 1] = '\0';
	if ((err = sanitize_filename(msg->payload, e)))
		return (err);
	if ((err = get_file_size(&size, e)))
		return (err);
	if ((err = prepare_transfer(msg->payload, size, &fd, &map)))
	{
		e->log(e, "put: %s: %s", error_get_string(err), strerror(errno));
		if (message_send(E_MESSAGE_ERR, strerror(err),
					ft_strlen(strerror(err)), e->csock) != E_ERR_OK)
		{
			e->should_quit = true;
			return (E_ERR_WRITE);
		}
		else
			return (err);
	}
	if ((err = message_send(E_MESSAGE_OK, NULL, 0, e->csock)))
		e->should_quit = true;
	else
		err = server_transfer(map, size, e);
	munmap(map, size);
	close(fd);
	return (err == E_ERR_OK);
}

/*
** client
*/

t_ecode client_transfer(void const *map, off_t size, t_env *e)
{
	if (sock_raw_write(e->csock, map, size) < 0)
	{
		e->log(e, "put: write(): %s", strerror(errno));
		e->should_quit = true;
		return (E_ERR_WRITE);
	}
	return (E_ERR_OK);
}

t_ecode	client_handshake(char const *filename, off_t size, t_env *e)
{
	t_ecode		err;
	t_message	*msg;

	e->should_quit = true;
	if ((err = message_send(E_MESSAGE_PUT, filename, ft_strlen(filename) + 1,
					e->csock)))
		return (err);
	if ((err = message_send(E_MESSAGE_PUT_SIZE, &size, sizeof(size), e->csock)))
		return (err);
	if ((err = message_receive(&msg, e->csock)))
		return (err);
	e->should_quit = false;
	if (msg->hd.op == E_MESSAGE_OK)
		err = E_ERR_OK;
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0 && msg->hd.size < INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
		err = E_ERR_SERVER;
	}
	else
		err = E_ERR_UNEXPECTED_OP;
	message_destroy(msg);
	return (err);
}

static t_ecode	prepare_file(char *path, int *dfd, struct stat *dbuf,
		void **dmap)
{
	int			saved_errno;

	if ((*dfd = open(path, O_RDONLY)) < 0)
		return (E_ERR_OPEN);
	if (fstat(*dfd, dbuf) < 0)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_FSTAT);
	}
	if ((*dmap = mmap(0, dbuf->st_size, PROT_READ, MAP_SHARED, *dfd, 0))
			== MAP_FAILED)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_MMAP);
	}
	return (E_ERR_OK);
}

t_bool	exec_cmd_put(char *const *args, char const **reason, t_env *e)
{
	int			fd;
	struct stat	buf;
	void		*map;
	t_ecode		err;

	if (!(err = prepare_file(args[1], &fd, &buf, &map)))
	{
		if (!(err = client_handshake(args[1], buf.st_size, e)))
			err = client_transfer(map, buf.st_size, e);
		munmap(map, buf.st_size);
		close(fd);
	}
	else
		e->log(e, "put: %s: %s", error_get_string(err), strerror(errno));
	if (err)
	{
		*reason = error_get_string(err);
		return (false);
	}
	return (true);
}

#include "common.h"
#include <dirent.h>

static const char	g_not_basename_err[] = "get: file must be a single path "
"component, relative to the current directory";

static const char	g_not_regular_err[] = "get: file is not a regular file";

//TODO: enhance to -> is_same_dir
t_bool	is_basename(char const *path)
{
	return (ft_strchr(path, '/') == NULL);
}

t_bool	is_regular_file_or_not_exist(char const *filename, t_env *e)
{
	DIR		*dirp;
	struct dirent	*dp;
	t_bool	ret;

	dirp = opendir(".");
	if (dirp == NULL)
	{
		e->log(e, "get: opendir() failed: %s", strerror(errno));
		return (message_send(E_MESSAGE_ERR, strerror(errno), ft_strlen(strerror(errno)), e->csock));
	}
	ret = true;
	while ((dp = readdir(dirp)) != NULL)
	{
		if (!strcmp(dp->d_name, filename))
		{
			if (dp->d_type != DT_REG)
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
	if (!is_regular_file_or_not_exist(filename, e))
	{
		if ((err = message_send(E_MESSAGE_ERR, g_not_regular_err,
					sizeof(g_not_regular_err), e->csock)))
			return (err);
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	return (E_ERR_OK);
}

/*
 * Do not allow relative paths in get. We cannot properly determine if the
 * target file is outside the root path.
 */
t_ecode	get_op_handler(t_message *msg, t_env *env)
{
	struct stat	buf;
	void		*map;
	int			fd;
	t_ecode		err;
	int			saved_errno;

	if (!msg->hd.size)
	{
		env->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	msg->payload[msg->hd.size - 1] = '\0';

	if ((err = sanitize_filename(msg->payload, env)))
		return (err);

	if ((fd = open(msg->payload, O_RDONLY)) < 0)
	{
		env->log(env, "get: %s: open failed - %s", msg->payload, strerror(errno));
		return (message_send(E_MESSAGE_ERR, strerror(errno),
					ft_strlen(strerror(errno)), env->csock));
	}

	if (fstat(fd, &buf) < 0)
	{
		saved_errno = errno;
		close(fd);
		env->log(env, "get: %s: fstat failed - %s", msg->payload, strerror(saved_errno));
		return (message_send(E_MESSAGE_ERR, strerror(saved_errno),
					ft_strlen(strerror(saved_errno)), env->csock));
	}

	if ((err = message_send(E_MESSAGE_OK, &buf.st_size, sizeof(buf.st_size), env->csock)))
	{
		close(fd);
		env->should_quit = true;
		return (E_ERR_WRITE);
	}

	if ((map = mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		saved_errno = errno;
		close(fd);
		return (message_send(E_MESSAGE_ERR, strerror(saved_errno),
					ft_strlen(strerror(saved_errno)), env->csock));
	}
	if (sock_raw_write(env->csock, map, buf.st_size) < 0)
	{
		//critical
		munmap(map, buf.st_size);
		close(fd);
		env->should_quit = true;
		return (E_ERR_WRITE);
	}

	munmap(map, buf.st_size);
	close(fd);
	return (E_ERR_OK);
}

static char const	*ft_basename(char const *path)
{
	char const	*dir;

	dir = ft_strrchr(path, '/');
	if (dir)
		return (dir + 1);
	else
		return (path);
}


/*
 * asks the server to prepare file download and retrieve its size.
 */
static t_bool	get_file_size(char const *filename, off_t *sizep,
		char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	if ((err = message_send(E_MESSAGE_GET,
					filename, ft_strlen(filename) + 1, e->csock)))
		return (command_abort(err, reason, &e->should_quit));
	if ((err = message_receive(&msg, e->csock)))
		return (command_abort(err, reason, &e->should_quit));
	ret = true;
	// not required. Makes sure struct stat.st_size is off_t
	assert(sizeof(*sizep) == sizeof(((struct stat *)NULL)->st_size));
	if (msg->hd.op == E_MESSAGE_OK && msg->hd.size == sizeof(*sizep))
	{
		ft_memcpy(sizep, msg->payload, sizeof(*sizep));
		ret = false;
	}
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0)
			e->log(e, "server: %.*s", msg->hd.size - 1, msg->payload);
		*reason = error_get_string(E_ERR_SERVER);
	}
	else
		*reason = error_get_string(E_ERR_UNEXPECTED_OP);
	message_destroy(msg);
	return (ret);
}

t_bool	exec_cmd_get(char * const *args, char const **reason, t_env *e)
{
	t_ecode		err;
	char const	*local_path;

	off_t	size;
	t_bool	ret;
	int	destfd;
	void	*map;

	if ((ret = get_file_size(args[1], &size, reason, e)))
	{
		close(destfd);
		return (false);
	}

	local_path = ft_basename(args[1]);
	if ((destfd = open(local_path, O_RDWR | O_CREAT, 0644)) < 0)
	{
		e->log(e, "get: open(): %s %s", args[1], strerror(errno));
		*reason = error_get_string(E_ERR_OPEN);
		return (false);
	}
	if (ftruncate(destfd, size) != 0)
	{
		e->log(e, "get: ftruncate(): %s", strerror(errno));
		close(destfd);
		//TODO v: we need to read what the server writes to keep in sync
		e->should_quit = true;
		*reason = error_get_string(E_ERR_FTRUNC);
		return(false);
	}
	if ((map = mmap(0, size, PROT_WRITE, MAP_SHARED, destfd, 0)) == MAP_FAILED)
	{
		e->log(e, "get: mmap(): %s", strerror(errno));
		close(destfd);
		//TODO v: We need to read what the server writes to keep in sync
		e->should_quit = true;
		*reason = error_get_string(E_ERR_MMAP);
		return (false);
	}
	if (sock_raw_read(e->csock, map, size) < 0)
	{
		e->log(e, "get: read(): %s", strerror(errno));
		munmap(map, size);
		close(destfd);
		e->should_quit = true;
		*reason = error_get_string(E_ERR_READ);
		return (false);
	}

	munmap(map, size);
	close(destfd);
	return (true);
}

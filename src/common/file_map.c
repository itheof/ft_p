/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 17:51:21 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 16:16:37 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

t_ecode	file_map_wr(char const *path, off_t size, int *dfd, void **dmap)
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

t_ecode	file_map_rd(char const *path, int *dfd, off_t *dsize, void **dmap)
{
	int			saved_errno;
	struct stat	buf;

	if ((*dfd = open(path, O_RDONLY)) < 0)
		return (E_ERR_OPEN);
	if (fstat(*dfd, &buf) < 0)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_FSTAT);
	}
	*dsize = buf.st_size;
	if ((*dmap = mmap(0, *dsize, PROT_READ, MAP_SHARED, *dfd, 0))
			== MAP_FAILED)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_MMAP);
	}
	return (E_ERR_OK);
}

void	file_unmap(int fd, off_t size, void *map)
{
	munmap(map, size);
	close(fd);
}

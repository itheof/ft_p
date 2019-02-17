/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 17:51:21 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 17:34:27 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

t_ecode	file_map_wr(char const *path, off_t size, t_map *map)
{
	int	saved_errno;

	map->size = size;
	if ((map->fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0644)) < 0)
		return (E_ERR_OPEN);
	if (ftruncate(map->fd, map->size) != 0)
	{
		saved_errno = errno;
		close(map->fd);
		errno = saved_errno;
		return (E_ERR_FTRUNC);
	}
	if ((map->data = mmap(0, map->size, PROT_WRITE, MAP_SHARED, map->fd, 0))
			== MAP_FAILED)
	{
		saved_errno = errno;
		close(map->fd);
		errno = saved_errno;
		return (E_ERR_MMAP);
	}
	return (E_ERR_OK);
}

t_ecode	file_map_rd(char const *path, t_map *map)
{
	int			saved_errno;
	struct stat	buf;

	if ((map->fd = open(path, O_RDONLY)) < 0)
		return (E_ERR_OPEN);
	if (fstat(map->fd, &buf) < 0)
	{
		saved_errno = errno;
		close(map->fd);
		errno = saved_errno;
		return (E_ERR_FSTAT);
	}
	map->size = buf.st_size;
	if ((map->data = mmap(0, map->size, PROT_READ, MAP_SHARED, map->fd, 0))
			== MAP_FAILED)
	{
		saved_errno = errno;
		close(map->fd);
		errno = saved_errno;
		return (E_ERR_MMAP);
	}
	return (E_ERR_OK);
}

void	file_unmap(t_map *map)
{
	munmap(map->data, map->size);
	close(map->fd);
}

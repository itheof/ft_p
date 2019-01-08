/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 11:00:38 by tvallee           #+#    #+#             */
/*   Updated: 2015/10/08 12:14:22 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFF_SIZE 1

typedef struct		s_buff
{
	char			*buff;
	int				offst;
	int				fd;
	int				readret;
	struct s_buff	*next;
}					t_buff;

int					get_next_line(int const fd, char **line);

#endif

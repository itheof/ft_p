/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_logger.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 11:06:12 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/20 11:39:28 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LOG_H
# define FT_LOG_H

# define C_EMERGENCY "\033[0;41;30m"
# define C_ALERT "\033[0;41;36m"
# define C_CRITICAL "\033[0;41;33m"
# define C_ERROR "\033[0;41;37m"
# define C_WARNING "\033[0;35m"
# define C_NOTICE "\033[0;33m"
# define C_INFO "\033[0;32m"
# define C_DEBUG "\033[0;34m"
# define C_REGULAR "\033[0m"

enum
{
	L_GETLVL = -1,
	L_EMERGENCY = 0,
	L_ALERT,
	L_CRITICAL,
	L_ERROR,
	L_WARNING,
	L_NOTICE,
	L_INFO,
	L_DEBUG,
	L_REGULAR
};

# define L_DEFAULT L_INFO

int		ft_set_log_lvl(int lvl);
int		ft_set_log_fd(int fd);
void	ft_log(int loglvl, char *msg);

#endif

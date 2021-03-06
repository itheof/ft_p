/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:48 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 13:05:59 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

extern t_command g_commands[COMMANDS_LEN];

t_bool	exec_cmd_help(char *const *args, char const **reason, t_env *e)
{
	size_t	n;

	(void)args;
	(void)e;
	(void)reason;
	n = 0;
	while (n < sizeof(g_commands) / sizeof(g_commands[0]))
	{
		printf("%-6s| %s\n", g_commands[n].name, g_commands[n].desc);
		n++;
	}
	return (true);
}

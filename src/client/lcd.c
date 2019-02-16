/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 17:25:45 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 17:26:03 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

t_bool			exec_cmd_lcd(char *const *args, char const **reason, t_env *e)
{
	if (chdir(args[1]) != 0)
	{
		perror("chdir");
		*reason = error_get_string(E_ERR_CHDIR);
		return (false);
	}
	free(e->cwd_path);
	if (!(e->cwd_path = getcwd(NULL, 0)))
	{
		perror("getcwd");
		*reason = error_get_string(E_ERR_GETCWD);
		e->should_quit = true;
		e->cwd_path = "programming error: cwd_path is not safe right now";
		return (false);
	}
	return (true);
}

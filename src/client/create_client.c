/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 18:10:04 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 18:11:53 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static int	err(char const *msg)
{
	printf("%s: %s\n", msg, strerror(errno));
	return (-1);
}

int			create_client(char const *addr, int port)
{
	struct protoent		*proto;
	struct hostent		*host;
	struct sockaddr_in	sin;
	int					sock;

	proto = getprotobyname("tcp");
	host = gethostbyname(addr);
	if (proto == 0 || !host)
		return (err("error"));
	assert(host->h_length == 4);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (sock < 0)
		return (err("socket"));
	else if (sock <= STDERR_FILENO)
	{
		printf("socket returned a standard FILENO. exiting to prevent hang\n");
		return (-1);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = *(uint32_t *)host->h_addr;
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		return (err("connect"));
	return (sock);
}

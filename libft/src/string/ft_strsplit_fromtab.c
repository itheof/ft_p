/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 14:51:18 by tvallee           #+#    #+#             */
/*   Updated: 2015/10/08 13:43:31 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t		count_words(char const *s, char *c)
{
	size_t	n;
	size_t	i;

	if (!s)
		return (0);
	n = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(c, s[i]))
			i++;
		if (s[i])
			n++;
		while (s[i] && !ft_strchr(c, s[i]))
			i++;
	}
	return (n);
}

static char const	*get_word(char const *s, char *c, size_t i)
{
	while (*s)
	{
		while (ft_strchr(c, *s))
			s++;
		if (!i)
			return (s);
		while (!ft_strchr(c, *s))
			s++;
		i--;
	}
	return (s);
}

static size_t		get_word_size(char const *s, char *c)
{
	size_t size;

	size = 0;
	while (*s && !ft_strchr(c, *s))
	{
		s++;
		size++;
	}
	return (size);
}

char				**ft_strsplit_fromtab(char const *s, char *c)
{
	size_t		n;
	char		**tab;
	size_t		i;
	char const	*current;

	if (s == NULL)
		ft_log(L_DEBUG, "caught NULL ptr in ft_strsplit");
	n = count_words(s, c);
	if ((tab = malloc((n + 1) * sizeof(*tab))) != NULL)
	{
		i = 0;
		while (i < n)
		{
			current = get_word(s, c, i);
			tab[i] = ft_strndup(current, get_word_size(current, c));
			i++;
		}
		tab[i] = NULL;
	}
	return (tab);
}

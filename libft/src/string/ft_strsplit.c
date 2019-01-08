/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 14:51:18 by tvallee           #+#    #+#             */
/*   Updated: 2015/10/08 12:55:09 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t		count_words(char const *s, char c)
{
	size_t	n;
	size_t	i;

	if (!s)
		return (0);
	n = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] != c)
			n++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (n);
}

static char const	*get_word(char const *s, char c, size_t i)
{
	while (*s)
	{
		while (*s == c)
			s++;
		if (!i)
			return (s);
		while (*s != c)
			s++;
		i--;
	}
	return (s);
}

static size_t		get_word_size(char const *s, char c)
{
	size_t size;

	size = 0;
	while (*s && *s != c)
	{
		s++;
		size++;
	}
	return (size);
}

char				**ft_strsplit(char const *s, char c)
{
	size_t		n;
	char		**tab;
	size_t		i;
	char const	*current;

	if (s == NULL)
		ft_log(L_DEBUG, "caught NULL ptr in ft_strsplit");
	n = count_words(s, c);
	tab = malloc((n + 1) * sizeof(*tab));
	i = 0;
	while (i < n)
	{
		current = get_word(s, c, i);
		tab[i] = ft_strndup(current, get_word_size(current, c));
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

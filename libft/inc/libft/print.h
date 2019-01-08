/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/10 12:14:55 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/20 11:45:21 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_H
# define PRINT_H

# include <stdlib.h>

# if defined(__MACH__) || defined(__linux__)
#  define FT_EOL(x) ft_putchar_fd('\n', x);
# else
#  define FT_EOL(x) ft_putstr_fd("\r\n", x);
# endif

void				ft_putchar(char c);
int					ft_putstr(char const *s);
void				ft_putendl(char const *s);
void				ft_putendlfree_fd(char *s, int fd);
void				ft_putnbr(int n);
void				ft_putunsigned(size_t n);
void				ft_puthex(size_t n);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_puthex_fd(size_t n, int fd);

void				ft_hexdump(const char *addr, size_t nbytes);

#endif

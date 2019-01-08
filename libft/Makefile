# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/12/10 14:41:44 by tvallee           #+#    #+#              #
#    Updated: 2018/04/26 16:30:39 by tvallee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = libft.a
CFLAGS         += -g -Wall -Wextra -Werror
SRCDIR          = src/
OBJDIR          = .obj/
INCDIR         = inc/

# set the suffix list explicitly
.SUFFIXES:
.SUFFIXES: .c .o .d

# list all sources
LIBC_SRC        = ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
                  ft_isdigit.c ft_isprint.c ft_isspace.c ft_memalloc.c \
                  ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c \
                  ft_memmove.c ft_memset.c ft_strcat.c ft_strchr.c ft_strcmp.c \
                  ft_strcpy.c ft_strdup.c ft_strlcat.c ft_strlen.c \
                  ft_strncat.c ft_strncmp.c ft_strncpy.c ft_strndup.c \
                  ft_strnstr.c ft_strrchr.c ft_strstr.c ft_tolower.c \
                  ft_toupper.c

LIST_SRC        = ft_atabtoistr.c ft_lstadd.c ft_lstdel.c ft_lstdelone.c \
                  ft_lstiter.c ft_lstlen.c ft_lstnew.c ft_lstnewcopy.c \
                  ft_lstpushback.c ft_lstremove.c ft_lsttotab.c \
                  ft_strlsttoitab.c

LOG_SRC         = ft_log.c

GETOPT_SRC      = ft_getopt.c ft_getopt_err.c

GNL_SRC         = get_next_line.c

PRINT_SRC       = ft_putchar.c ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c \
                  ft_putendlfree_fd.c ft_putnbr.c ft_putnbr_fd.c ft_putstr.c \
                  ft_putstr_fd.c ft_puthex.c ft_puthex_fd.c ft_putunsigned.c \
				  ft_hexdump.c

STRING_SRC      = ft_strclr.c ft_strdel.c ft_strequ.c ft_striter.c \
                  ft_striteri.c ft_strjoin.c ft_strmap.c ft_strmapi.c \
                  ft_strnequ.c ft_strnew.c ft_strsplit.c ft_strsplit_fromtab.c \
                  ft_strsub.c ft_strtrim.c

ARRAY_SRC       = ft_tabfree.c ft_tablen.c ft_tablineadd.c ft_tablinedel.c \
                  ft_tabtolst.c

#TREE_SRC        = #btree_apply_infix.c btree_apply_prefix.c \
#                  btree_apply_suffix.c btree_create_node.c btree_insert_data.c

BUFFER_SRC      = buffer_init.c buffer_cat.c buffer_deinit.c buffer_reserve.c \
				  buffer_reserve.c buffer_cat_num.c

MISC_SRC        = ft_itoa.c ft_sizealign.c

SWAP_SRC        = swap_short.c swap_long.c swap_longlong.c


SRCDIRS         = libc/ list/ log/ get_next_line/ print/ string/ array/ tree/ misc/ getopt/ buffer/ swap/
SRC             = $(addprefix libc/,$(LIBC_SRC)) \
				  $(addprefix list/,$(LIST_SRC)) \
				  $(addprefix log/,$(LOG_SRC)) \
                  $(addprefix get_next_line/,$(GNL_SRC)) \
				  $(addprefix print/,$(PRINT_SRC)) \
				  $(addprefix string/,$(STRING_SRC)) \
				  $(addprefix array/,$(ARRAY_SRC)) \
				  $(addprefix tree/,$(TREE_SRC)) \
				  $(addprefix misc/,$(MISC_SRC)) \
				  $(addprefix getopt/,$(GETOPT_SRC)) \
				  $(addprefix buffer/,$(BUFFER_SRC)) \
				  $(addprefix swap/,$(SWAP_SRC))


CFLAGS         += -I$(INCDIR)
OBJS            = $(SRC:.c=.o)
DEPS            = $(OBJS:.o=.d)
OUTPUT_OPTION   = -o $(OBJDIR)$@
CPPFLAGS       += -MT $@ -MMD -MP -MF $(OBJDIR)$(@:.o=.d)
OBJDIRS         = $(addprefix $(OBJDIR),$(SRCDIRS))

vpath %.c $(SRCDIR)
vpath %.o $(OBJDIR)

$(NAME): $(OBJS)
	$(AR) rcs $@ $(addprefix $(OBJDIR),$(OBJS))

.SECONDEXPANSION:
$(OBJS): %: | $$(dir $(OBJDIR)%)
-include $(addprefix $(OBJDIR),$(DEPS))

$(OBJDIRS) $(OBJDIR):
	mkdir -p $@

clean:
	$(RM) -Rf $(OBJDIR)

fclean: | clean
	$(RM) -f $(NAME)

re:
	$(MAKE) -j8 fclean
	$(MAKE) -j8

.PHONY: clean fclean re

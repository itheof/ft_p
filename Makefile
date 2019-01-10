NAME         = server client
CFLAGS      += -g -Wall -Wextra #-Werror
CFLAGS      += -std=c11 -pedantic -pedantic-errors

ifeq ($(DEBUG),yes)
	CFLAGS  += -g3 -O0 -fno-inline -DNOT_CORRECTION
else
	CFLAGS  += -O2 -DNDEBUG
endif
ifeq ($(SAN),yes)
	CFLAGS	+= -fsanitize=address
	LDFLAGS += -fsanitize=address
endif
CFLAGS  += $(TMP_CFLAGS)

# Headers
CFLAGS    += -I./inc


COM_SOURCES    = $(addprefix common/,\
				 common.c\
				 ping.c\
				 message.c\
				 error.c\
				 sock_raw.c)

CLIENT_SOURCES = $(addprefix client/,\
				 client.c\
				 help.c\
				 command.c)

SERVER_SOURCES = $(addprefix server/,\
				 misc.c\
				 master_init.c\
				 worker.c\
				 server.c)

# Sources
SRC_PATH    = src

SOURCES     = $(COM_SOURCES) $(CLIENT_SOURCES) $(SERVER_SOURCES)

# Generation
vpath %.c $(SRC_PATH)
OBJ_PATH    = .obj/
COM_OBJECTS = $(COM_SOURCES:%.c=$(OBJ_PATH)%.o)
CLIENT_OBJECTS = $(CLIENT_SOURCES:%.c=$(OBJ_PATH)%.o)
SERVER_OBJECTS = $(SERVER_SOURCES:%.c=$(OBJ_PATH)%.o)
OBJECTS     = $(SOURCES:%.c=$(OBJ_PATH)%.o)
DEP_PATH    = .dep/
DEPS        = $(SOURCES:%.c=$(DEP_PATH)%.d)
BUILD_DIRS	= .dep/client .dep/server .dep/common \
			  .obj/client .obj/server .obj/common

# Libft
LIBFT_PATH = libft
LIBFT      = $(LIBFT_PATH)/libft.a
CFLAGS    += -I $(LIBFT_PATH)/inc
LDFLAGS   += -L $(LIBFT_PATH) -lft

all: $(DEPS) $(NAME)

-include $(DEPS)

server: $(COM_OBJECTS) $(SERVER_OBJECTS) $(LIBFT)
	$(CC) $(LDFLAGS) -o $@ $^
client: $(COM_OBJECTS) $(CLIENT_OBJECTS) $(LIBFT)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(OBJ_PATH)%.o: %.c
	@mkdir -p $(BUILD_DIRS) #FIXME
	$(CC) $(CFLAGS) -o $@ -c $<

$(DEPS): $(DEP_PATH)%.d: %.c
	@mkdir -p $(BUILD_DIRS) #FIXME
	$(CC) $(CFLAGS) -MM $< -MT $(OBJ_PATH)$*.o -MF $@

clean:
	$(RM) -rf $(BUILD_DIRS) $(TESTS)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) -rf $(NAME)
	$(MAKE) -C $(LIBFT_PATH) fclean

re:
	$(MAKE) fclean
	$(MAKE) all

# The rule libft must be triggered manually
$(LIBFT):
	@$(MAKE) -q -C $(LIBFT_PATH) || echo $(MAKE) -C $(LIBFT_PATH) && \
		$(MAKE) -j8 -C $(LIBFT_PATH)

.PHONY: all clean fclean re #$(LIBFT)

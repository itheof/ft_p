#ifndef ERROR_H
# define ERROR_H

# include "libft/stdbool.h"

typedef enum	e_ecode
{
	E_ERR_OK = false,
	E_ERR_CLOSED,
	E_ERR_INCOMPLETE_HEADER,
	E_ERR_INCOMPLETE_PAYLOAD,
	E_ERR_UNIMPLEMENTED_OP,
	E_ERR_INVALID_OP,
	E_ERR_READ,
	E_ERR_WRITE,
	E_ERR_MALLOC,
	E_ERR_MAX,
}				t_ecode;

char const	*error_get_string(t_ecode e);

#endif

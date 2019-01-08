#include <assert.h>
#include "error.h"

static char const	*(g_err_desc[]) = {
	[E_ERR_OK] = "success",
	[E_ERR_CLOSED] = "connection closed by peer",
	[E_ERR_INCOMPLETE_HEADER] = "received partial FTp header",
	[E_ERR_INCOMPLETE_PAYLOAD] = "received partial FTp payload",
	[E_ERR_INVALID_OP] = "received invalid opcode",
	[E_ERR_READ] = "read error",
	[E_ERR_WRITE] = "write error",
	[E_ERR_MALLOC] = "malloc error",
};

char const	*error_get_string(t_ecode e)
{
	assert(e >= 0 && e < E_ERR_MAX
			&& e < sizeof(g_err_desc) / sizeof(g_err_desc[0]));
	return (g_err_desc[e]);
}

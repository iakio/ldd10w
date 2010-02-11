#include "postgres.h"
#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(func);
extern Datum func(PG_FUNCTION_ARGS);

Datum
func(PG_FUNCTION_ARGS)
{
	int32	i = PG_GETARG_INT32(0);
	PG_RETURN_INT32(i);
}

#include "postgres.h"
#include "fmgr.h"
#include "lib/stringinfo.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(func);
PG_FUNCTION_INFO_V1(gcd);
PG_FUNCTION_INFO_V1(rat_in);
PG_FUNCTION_INFO_V1(rat_out);
PG_FUNCTION_INFO_V1(rat_add);
PG_FUNCTION_INFO_V1(rat_sub);
PG_FUNCTION_INFO_V1(rat_mul);
PG_FUNCTION_INFO_V1(rat_div);
PG_FUNCTION_INFO_V1(i4torat);
extern Datum func(PG_FUNCTION_ARGS);
extern Datum gcd(PG_FUNCTION_ARGS);
extern Datum rat_in(PG_FUNCTION_ARGS);
extern Datum rat_out(PG_FUNCTION_ARGS);
extern Datum rat_add(PG_FUNCTION_ARGS);
extern Datum rat_sub(PG_FUNCTION_ARGS);
extern Datum rat_mul(PG_FUNCTION_ARGS);
extern Datum rat_div(PG_FUNCTION_ARGS);
extern Datum i4torat(PG_FUNCTION_ARGS);

typedef struct {
	int32	num;
	int32	denom;
} Rat;

Datum
func(PG_FUNCTION_ARGS)
{
	int32	i = PG_GETARG_INT32(0);
	PG_RETURN_INT32(i);
}


static int32
gcd_internal(int32 a, int32 b)
{
	int32		c;
	while (b != 0) {
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}

Datum
gcd(PG_FUNCTION_ARGS)
{
	int32	a = PG_GETARG_INT32(0),
			b = PG_GETARG_INT32(1);

	/* TODO */
	PG_RETURN_INT32(gcd_internal(a, b));
}

Datum
rat_in(PG_FUNCTION_ARGS)
{
	char	*in = PG_GETARG_CSTRING(0);
	Rat		*out = (Rat *) palloc(sizeof(Rat));

	sscanf(in, "%d/%d", &out->num, &out->denom);

	PG_RETURN_POINTER(out);
}

Datum
rat_out(PG_FUNCTION_ARGS)
{
	Rat		*in = (Rat *) PG_GETARG_POINTER(0);
	StringInfoData str;

	initStringInfo(&str);
	appendStringInfo(&str, "%d/%d", in->num, in->denom);

	PG_RETURN_CSTRING(str.data);
}

static Rat*
rat_create(int32 num, int32 denom)
{
	Rat	*rat = (Rat *) palloc(sizeof(Rat));
	int32	g = gcd_internal(num, denom);

	rat->num = num / g;
	rat->denom = denom / g;
	return rat;
}

Datum
rat_add(PG_FUNCTION_ARGS)
{
	Rat		*r1 = (Rat *) PG_GETARG_POINTER(0),
			*r2 = (Rat *) PG_GETARG_POINTER(1);

	PG_RETURN_POINTER(
		rat_create(
			r1->num * r2->denom + r1->denom * r2->num,
			r1->denom * r2->denom
		)
	);
}

Datum
rat_sub(PG_FUNCTION_ARGS)
{
	Rat		*r1 = (Rat *) PG_GETARG_POINTER(0),
			*r2 = (Rat *) PG_GETARG_POINTER(1);

	PG_RETURN_POINTER(
		rat_create(
			r1->num * r2->denom - r1->denom * r2->num,
			r1->denom * r2->denom
		)
	);
}

Datum
rat_mul(PG_FUNCTION_ARGS)
{
	Rat		*r1 = (Rat *) PG_GETARG_POINTER(0),
			*r2 = (Rat *) PG_GETARG_POINTER(1);

	PG_RETURN_POINTER(
		rat_create(
			r1->num * r2->num,
			r1->denom * r2->denom
		)
	);
}

Datum
rat_div(PG_FUNCTION_ARGS)
{
	Rat		*r1 = (Rat *) PG_GETARG_POINTER(0),
			*r2 = (Rat *) PG_GETARG_POINTER(1);

	PG_RETURN_POINTER(
		rat_create(
			r1->num * r2->denom,
			r1->denom * r2->num
		)
	);
}

Datum
i4torat(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(rat_create(PG_GETARG_INT32(0), 1));
}
/* vim: set path+=~/pgsql/include/server :*/

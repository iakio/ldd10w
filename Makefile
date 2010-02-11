OBJS		= ldd10w.o

MODULE_big	= ldd10w
#DOCS		= README.ldd10w
#REGRESS		=
DATA_built	= ldd10w.sql
PGXS		= $(shell pg_config --pgxs)
include $(PGXS)

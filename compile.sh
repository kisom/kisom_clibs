#!/bin/ksh


# VARIABLES
CC="gcc"
CCFLAGS="-c -Wall -o2"
LIBDIR=lib
SRCDIR=src

# hostipn
$CC ${CCFLAGS} -o ${LIBDIR}/hostipn.o ${SRCDIR}/hostipn.c

# ssl_common
$CC ${CCFLAGS} -o ${LIBDIR}/ssl_common.o ${SRCDIR}/ssl_common.c



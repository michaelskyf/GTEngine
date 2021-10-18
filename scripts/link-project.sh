#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-only

# Error out on error
set -e

LD="$1"
PBUILD_LDFLAGS="$2"
LDFLAGS_project="$3"

# Nice output in kbuild format
# Will be supressed by "make -s"
info()
{
	printf "  %-7s %s\n" "${1}" "${2}"
}

# ${1} output file
modpost_link()
{
	local objects
	local lds=""

	objects="${PBUILD_PROJECT_OBJS} ${PBUILD_PROJECT_LIBS}"

		info LD ${1}

		gcc ${PBUILD_LDFLAGS} -o ${1} ${lds} ${objects}
}

# Link of project
# ${1} - output file
# ${2}, ${3}, ... - optional extra .o files
project_link()
{
    local output=${1}
	local objs
	local libs
	local ld
	local ldflags
	local ldlibs

    info LD ${output}

    # skip output file argument
	shift

    objs=${PBUILD_PROJECTNAME}.o
	libs=
	wl=
	ld="${LD}"
	ldflags="${PBUILD_LDFLAGS} ${LDFLAGS_project}"
	ldlibs=

    ldflags="${ldflags} ${wl}--script=${objtree}/${PBUILD_LDS}"

	${ld} -o ${output} ${objs} $@ ${ldlibs}
}

# Delete output files in case of error
cleanup()
{
	rm -f .tmp_*
	rm -f ${PBUILD_PROJECTNAME}
	rm -f ${PBUILD_PROJECTNAME}.map
	rm -f ${PBUILD_PROJECTNAME}.o
	rm -f .${PBUILD_PROJECTNAME}.d
}

# Use "make V=1" to debug this script
case "${PBUILD_VERBOSE}" in
*1*)
	set -x
	;;
esac

if [ "$1" = "clean" ]; then
	cleanup
	exit 0
fi

#link vmlinux.o
modpost_link ${PBUILD_PROJECTNAME}

# For fixdep
echo "${PBUILD_PROJECTNAME}: $0" > .${PBUILD_PROJECTNAME}.d

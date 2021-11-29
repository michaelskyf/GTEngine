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
project_link()
{
	local objects
	local lds=""

	objects="-Wl,--whole-archive				\
		${PBUILD_PROJECT_OBJS}				\
		-Wl,--no-whole-archive				\
		-Wl,--start-group				\
		${PBUILD_PROJECT_LIBS}				\
		-Wl,--end-group"

		info LINK ${1}

		${LD} -o ${1} ${lds} ${objects} ${PBUILD_LDFLAGS}
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

# link project
project_link ${PBUILD_PROJECTNAME}

# For fixdep
echo "${PBUILD_PROJECTNAME}: $0" > .${PBUILD_PROJECTNAME}.d

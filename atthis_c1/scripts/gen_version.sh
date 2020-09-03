#!/bin/sh
# ----------------------------------------------------
# This script auto-generate faceid_version.h according to metadata
# Usage: ./gen_version.sh
# ----------------------------------------------------
VERSION_NUM=$(awk -F= '{if($1=="version") print $2}' ./../metadata)
: ${VERSON_NUM:=0.0.1}

MICRO_VERSION_SUFFIX=

COMPILE_BY=`whoami`
COMPILE_HOST=`hostname`
COMPILE_TIME=`date +%T`
COMPILE_DATE=`date +%F`

get_version()
{
    if [ -d .git -o \
         -d ../.git -o \
         -d ../../.git -o \
         -d ../../../.git -o \
         -d ../../../../.git -o \
         -d ../../../../../.git ]; then
        extra="$(git rev-parse --verify --short HEAD)"
        VERSION_EXTRA="-$extra"
        if git diff-index --name-only HEAD |  grep -v "^scripts/package" | read dummy ; then
            VERSION_EXTRA="${VERSION_EXTRA}-dirty!"
        else
            VERSION_EXTRA="${VERSION_EXTRA}-commit"
        fi
    else
        echo -e "\n\n\033[31mERROR: oh no git found\033[37m\n\n"
        exit
    fi
}

get_version

VERSION=$VERSION_NUM$VERSION_EXTRA

cat > inc/helper/version.h <<_ACEOF
/******************************************************************************
  * @copyright All code copyright Baidu Authors 2020-2021, all rights reserved.
 *
 * @brief
 *
 *****************************************************************************/
#ifndef _VERSION_H
#define _VERSION_H

#define ATTHIS_VERSION "V$VERSION"
#define COMPILE_BY "$COMPILE_BY"
#define COMPILE_HOST "$COMPILE_HOST"
#define COMPILE_TIME "$COMPILE_TIME"
#define COMPILE_DATE "$COMPILE_DATE"

#endif  // _VERSION_H
_ACEOF

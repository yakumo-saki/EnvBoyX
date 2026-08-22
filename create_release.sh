#!/bin/bash -eu

MYDIR=$(cd "$(dirname "$0")"; pwd)
RELEASE_BASE_DIR=${MYDIR}/_release
SRC_BUILD_DIR=${MYDIR}/.pio/build

SED=sed
if [ "$(uname)" == 'Darwin' ]; then
  SED=gsed  # install via homebrew install gnu-sed
fi
# SUBs

banner () {
  echo ""
  echo "================================================="
  echo "$1"
  echo "================================================="
}

copy_src_to_tgt () {
  banner "Copying board build $1"

  # $1 = boardname
  cp -v ${SRC_BUILD_DIR}/$1/firmware.bin ${RELEASE_BASE_DIR}/$1_EnvBoyX_$VER$VER_ADDITIONAL.bin
}

# --------------------
# MAIN
# --------------------
cd ${MYDIR}

# -------------------------------------------------------------------------
banner "Get Version"
# -------------------------------------------------------------------------
MAJOR=$(grep " ver =" ./src/global.cpp | $SED -r 's/.*"(.*+)".*/\1/')
MINOR=$(grep " minorVer =" ./src/global.cpp | $SED -r 's/.*"(.*+)".*/\1/')
VER=$(echo "${MAJOR}.${MINOR}")
echo "Version is ${VER}"


# -------------------------------------------------------------------------
banner "Get Version additional info (debug, branch...)"
# -------------------------------------------------------------------------
# git ブランチからファイル名判定
branch=`git symbolic-ref --short HEAD`
VER_ADDITIONAL=""
if [ $branch != "main" ]; then
  echo "git branch is not master/main!"
  echo "Appending branch name"
  VER_ADDITIONAL=$branch
fi

# DEBUGビルド判定
set +e
grep -i "DEBUG_BUILD.*TRUE" ${MYDIR}/src/global.cpp
IS_DEBUG=$?

if [ $IS_DEBUG -eq 0 ] ; then
  grep -i "const DEBUG_MODE.*true" ${MYDIR}/embed/static/config.js
  IS_DEBUG=$?
fi
set -e

if [ $IS_DEBUG -eq 0 ] ; then
  echo "** DEBUG_BUILD IS SET TO TRUE"

  if [ ${#VER_ADDITIONAL} -eq 0 ] ; then
    VER_ADDITIONAL=DEBUG
  else
    VER_ADDITIONAL=$(echo "${VER_ADDITIONAL}-DEBUG")
  fi
fi

if [ ${#VER_ADDITIONAL} -gt 0 ] ; then
  VER_ADDITIONAL=$(echo "-$VER_ADDITIONAL")
fi

echo "Version Additional is '$VER_ADDITIONAL'"


# -------------------------------------------------------------------------
banner "Delete and create build directory"
# -------------------------------------------------------------------------
rm -rf ${RELEASE_BASE_DIR}
mkdir -p ${RELEASE_BASE_DIR}

# -------------------------------------------------------------------------
banner "Build"
# -------------------------------------------------------------------------

BOARDS=( esp32dev esp12e )

pio run --target clean
pio run --environment esp32dev  --environment esp12e

for b in ${BOARDS[@]} ; do
  copy_src_to_tgt $b
done


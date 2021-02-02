#!/bin/bash -eu

MYDIR=$(cd "$(dirname "$0")"; pwd)
RELEASE_BASE_DIR=${MYDIR}/_release/
RELEASE_DIR=${RELEASE_BASE_DIR}/EnvBoyX
SRC_BUILD_DIR=${MYDIR}/.pio/build
TGT_BUILD_DIR=${RELEASE_DIR}/.pio/build

banner () {
  echo ""
  echo "================================================="
  echo "$1"
  echo "================================================="
}

copy_src_to_tgt () {
  banner "Copying board build $1"

  # ESP32 dev board
  mkdir -p ${TGT_BUILD_DIR}/$1
  cp -v ${SRC_BUILD_DIR}/$1/*.bin ${TGT_BUILD_DIR}/$1/
}

banner "Delete source build dir and clean build"
#rm -rf ${MYDIR}/.pio
#cd ${MYDIR}
#pio run

banner "Delete and create build directory"
rm -rf ${RELEASE_DIR}

mkdir -p ${RELEASE_DIR}

cp ./platformio.ini ${RELEASE_DIR}/

BOARDS=( esp32dev esp12e )

for b in ${BOARDS[@]} ; do
  copy_src_to_tgt $b
done

banner "Creating Archive"

# git ブランチからファイル名判定
branch=`git branch --contains | cut -b3-`

if [ $branch = "master" ]; then
  RELEASE_FILE_PATH=${MYDIR}/EnvBoyX.tar.gz
elif [ $branch = "main" ]; then
  RELEASE_FILE_PATH=${MYDIR}/EnvBoyX.tar.gz
else
  echo "git branch is not master/main!"
  echo "Appending branch name"
  RELEASE_FILE_PATH=${MYDIR}/EnvBoyX-$branch.tar.gz
fi

rm -f ${RELEASE_FILE_PATH}
tar -C ${RELEASE_BASE_DIR} -zcvf ${RELEASE_FILE_PATH} .

banner "Release archive created ${RELEASE_FILE_PATH}"

echo "To upload, run:"
echo "tar xvf `basename ${RELEASE_FILE_PATH}`"
echo "cd `basename ${RELEASE_FILE_PATH}`"
echo "pio run -t nobuild -t upload --disable-auto-clean"
echo "If you want to specify board, use below."
echo "pio run -e [esp32dev | esp12e] -t nobuild -t upload --disable-auto-clean"
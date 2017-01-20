#!/bin/sh
#
export PATH=/cygdrive/c/cygwin64/bin:$PATH
echo `pwd`
echo "preset VCVerChanger_BINARY: " $BINARY

TARGET_URL=${BASE_URL}/${BINARY}

RUNTIME_DIR=changer_runtime
RUNTIME_ZIP=${RUNTIME_DIR}.zip

TARGET_DIR=`basename $BINARY .zip`
echo "preset TARGET_DIR: " $TARGET_DIR

rm -rf $TARGET_DIR $BINARY $RUNTIME_DIR
wget $TARGET_URL
unzip $BINARY
chmod -R 755 $TARGET_DIR
unzip $RUNTIME_ZIP
chmod -R 755 $RUNTIME_DIR

cp ${TARGET_DIR}/*.exe ${RUNTIME_DIR}/utils/$TARGET_DIR/

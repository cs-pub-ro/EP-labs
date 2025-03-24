#!/bin/bash

PIN_URL="https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.24-98612-g6bd5931f2-gcc-linux.tar.gz"
ARCHIVE="$(basename ${PIN_URL})"

mkdir -p third_party
pushd third_party

curl -o ${ARCHIVE} ${PIN_URL}

tar xf ${ARCHIVE}
rm -f ${ARCHIVE}
mv pin-* pin-3.24

popd
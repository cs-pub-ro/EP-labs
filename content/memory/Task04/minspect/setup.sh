#!/bin/bash

PIN_URL="https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.24-98612-g6bd5931f2-gcc-linux.tar.gz"
ARCHIVE="$(basename ${PIN_URL})"

# fetch intel pin
mkdir -p third_party
pushd third_party

wget -U 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)' ${PIN_URL}
tar xf ${ARCHIVE}
rm -f ${ARCHIVE}
mv pin-* pin-3.24

popd

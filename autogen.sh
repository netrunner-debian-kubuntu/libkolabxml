#!/bin/bash

dobuild=0
doprep=0
dotest=0
doinstall=0

srcdir=$(pwd)

while [ $# -gt 0 ]; do
    case "$1" in
        --build|-b)
            dobuild=1
            shift
        ;;

        --prep|-p)
            doprep=1
            shift
        ;;

        --test|-t)
            dotest=1
            shift
        ;;

        --install|-i)
            doinstall=1
            shift
        ;;

    esac
done

if [ ${dobuild} -eq 0 -a ${doprep} -eq 0 -a ${dotest} -eq 0 -a ${doinstall} -eq 0 ]; then
    dobuild=1
    doprep=1
    dotest=1
    doinstall=1
fi

version_major=`grep -E "^set \(Libkolabxml_VERSION_MAJOR [0-9]+\)" CMakeLists.txt | sed -r -e 's/^set \(Libkolabxml_VERSION_MAJOR ([0-9]+)\)/\1/g'`
version_minor=`grep -E "^set \(Libkolabxml_VERSION_MINOR [0-9]+\)" CMakeLists.txt | sed -r -e 's/^set \(Libkolabxml_VERSION_MINOR ([0-9]+)\)/\1/g'`
version_patch=`grep -E "^set \(Libkolabxml_VERSION_PATCH [0-9]+\)" CMakeLists.txt | sed -r -e 's/^set \(Libkolabxml_VERSION_PATCH ([0-9]+)\)/\1/g'`

if [ -z "${version_patch}" ]; then
    version="${version_major}.${version_minor}"
else
    version="${version_major}.${version_minor}.${version_patch}"
fi

# Rebuilds the entire foo in one go. One shot, one kill.
if [ ${doprep} -eq 1 ]; then
    rm -rf build/
    mkdir -p build
fi

cd build

if [ ${doprep} -eq 1 ]; then
    cmake \
        -DCMAKE_COLOR_MAKEFILE=OFF \
        -DCMAKE_VERBOSE_MAKEFILE=ON \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DINCLUDE_INSTALL_DIR=/usr/include \
        -DLIB_INSTALL_DIR=/usr/lib64 \
        -DCSHARP_BINDINGS=ON \
        -DJAVA_BINDINGS=ON \
        -DPHP_BINDINGS=ON \
        -DPYTHON_BINDINGS=ON \
        .. || exit 1
fi

if [ ${dobuild} -eq 1 ]; then
    make || exit 1
fi

if [ ${dotest} -eq 1 ]; then
    # Execute some tests?

    retval=0

    pushd tests
    ./bindingstest ; retval=$(( ${retval} + $? ))
    ./conversiontest ; retval=$(( ${retval} + $? ))
    ./parsingtest ; retval=$(( ${retval} + $? ))
    popd

    if [ -f "${srcdir}/build/src/csharp/test.exe" ]; then
       LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/build/src/csharp/ MONO_LOG_LEVEL=debug mono ${srcdir}/build/src/csharp/test.exe ; retval=$(( ${retval} + $? ))
    fi

    if [ -f "${srcdir}/build/src/php/test.php" ]; then
        php -d enable_dl=On -dextension=${srcdir}/build/src/php/kolabformat.so ${srcdir}/build/src/php/test.php ; retval=$(( ${retval} + $? ))
    fi

    if [ -f "${srcdir}/build/src/python/test.py" ]; then
        python ${srcdir}/build/src/python/test.py ; retval=$(( ${retval} + $? ))
    fi

    if [ ${retval} -ne 0 ]; then
        echo "FAILED: Accumulated errors."
        exit ${retval}
    fi
fi

if [ ${doinstall} -eq 1 ]; then
    make install DESTDIR=${TMPDIR:-/tmp}
fi

cd ..

rm -rf libkolabxml-${version}.tar.gz
git archive --prefix=libkolabxml-${version}/ HEAD | gzip -c > libkolabxml-${version}.tar.gz

rm -rf `rpm --eval='%{_sourcedir}'`/libkolabxml-${version}.tar.gz
cp libkolabxml-${version}.tar.gz `rpm --eval='%{_sourcedir}'`


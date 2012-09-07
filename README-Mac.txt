# The following commands are used to bild libkolabxml on Mac OS X (Lion)
# with most of the required libraries installed via macports:
# > sudo port install cmake boost xercesc3
#
# The cxx/xsd library has to be downlaoded from http://www.codesynthesis.com/products/xsd/download.xhtml
# and manually copied into /usr/bin and /usr/include/xsd/

rm -rf build/
mkdir -p build
cd build
cmake -DPHP_BINDINGS=TRUE -DPHP_INSTALL_DIR:PATH=/usr/lib/php/extensions/no-debug-non-zts-20090626 -DCMAKE_INCLUDE_PATH:PATH=/opt/local/include -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
make
sudo make install

# This will install kolabformat.dylib and kolabform.php to the KOLAB_PHP_INSTALL_DIR
# You probably have to symlink kolabformat.dylib to kolabformat.so and move
# kolabform.php to a location which is in the include_path of your PHP installation.

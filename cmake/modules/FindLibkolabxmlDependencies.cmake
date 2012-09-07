# Find some dependencies
# Instead of having multiple findscripts, we use a single to find everything we need
#
find_package(PkgConfig)
include(FindPackageHandleStandardArgs)

if (${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION} VERSION_LESS 1.42)
    find_library(UUID NAMES ossp-uuid)
    find_package_handle_standard_args(UUID DEFAULT_MSG
                                  UUID)
    if (UUID)
        set(UUID_LIBRARY_FOUND TRUE)
    endif()
else()
    #Make sure that we either have the boost or ossp uuid lib
    set(UUID_LIBRARY_FOUND TRUE)
endif()

find_program(XSDCXX NAMES xsdcxx xsd /usr/bin/)
if (XSDCXX)
    find_path(XSDCXX_INCLUDE_DIRS NAMES xsd/cxx/version.hxx)
endif()
find_package_handle_standard_args(XSDCXX  DEFAULT_MSG
                                  XSDCXX XSDCXX_INCLUDE_DIRS)

find_library(XERCES_C NAMES xerces-c xerces-c_2)
if (XERCES_C)
    find_path(XERCES_C_INCLUDE_DIRS NAMES xercesc/framework/XMLGrammarPool.hpp)
endif()
find_package_handle_standard_args(Xerces  DEFAULT_MSG
                                  XERCES_C XERCES_C_INCLUDE_DIRS)

find_library(CURL NAMES curl)
if (CURL)
    find_path(CURL_INCLUDE_DIRS NAMES curl/curl.h)
endif()
find_package_handle_standard_args(Curl  DEFAULT_MSG
                                  CURL CURL_INCLUDE_DIRS)

find_program(SWIG swig /usr/bin/)
if(SWIG)
    set(SWIG_FOUND ON)
    message("SWIG found")
endif()

find_program(XSDBIN xsdbin)
if(XSDBIN)
    set(XSDBIN_FOUND ON)
    message("XSDBIN found")
endif()

#abort if any of the requireds are missing
find_package_handle_standard_args(LibkolabxmlDependencies  DEFAULT_MSG
                                  UUID_LIBRARY_FOUND XSDCXX XERCES_C CURL)

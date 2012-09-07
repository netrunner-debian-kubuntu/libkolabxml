# The special language NONE is not available in cmake 2.6
if(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 6)
    project(Libkolabxml NONE)
else()
    project(Libkolabxml C)
endif()

cmake_minimum_required(VERSION 2.6)

option( BUILD_TESTS "Build the tests" TRUE )
option( DIST_ONLY "Build dist targets only (does not require a compiler)" FALSE )
option( PYTHON_BINDINGS "Build bindings for python" FALSE )
option( PHP_BINDINGS "Build bindings for php" FALSE )
option( CSHARP_BINDINGS "Build bindings for csharp" FALSE )
option( JAVA_BINDINGS "Build bindings for java" FALSE )


set(Libkolabxml_MODULE_DIR ${Libkolabxml_SOURCE_DIR}/cmake/modules)
set(CMAKE_MODULE_PATH ${Libkolabxml_MODULE_DIR})

# only available from cmake-2.8.0
if(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 7)
    cmake_policy(SET CMP0012 NEW)
endif()

# only available from cmake-2.8.4
if(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 7 AND
    ${CMAKE_PATCH_VERSION} GREATER 3)
    cmake_policy(SET CMP0017 NEW)
endif()

# Versioning
# x.y.z scheme
# Development versions are only x.y
#
# i.e.
# 0.1 (0.1 development version)
# 0.1.0 (first release)
# 0.1.1 (patch release for 0.1.0)
# 0.2 (0.2 development version)

set (Libkolabxml_VERSION_MAJOR 0)
set (Libkolabxml_VERSION_MINOR 8)
# Enable the full x.y.z version only for release versions
set (Libkolabxml_VERSION_PATCH 1)
set (Libkolabxml_VERSION "${Libkolabxml_VERSION_MAJOR}.${Libkolabxml_VERSION_MINOR}.${Libkolabxml_VERSION_PATCH}" )
# set (Libkolabxml_VERSION "${Libkolabxml_VERSION_MAJOR}.${Libkolabxml_VERSION_MINOR}" )

set (Libkolabxml_VERSION_STRING ${CMAKE_PROJECT_NAME}-${Libkolabxml_VERSION})
set (KOLAB_FORMAT_VERSION "3.0dev1")

message("Building DIST targets (make dist, make snapshot)")
#make dist requires a tag with ${ARCHIVE_NAME} (e.g. libkolabxml-0.2.0)
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${Libkolabxml_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ ${ARCHIVE_NAME}
        | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

#snapshot of current development version
set(SNAPSHOTARCHIVE_NAME "${CMAKE_PROJECT_NAME}-${Libkolabxml_VERSION}-HEAD")
add_custom_target(snapshot
    COMMAND git archive --prefix=${SNAPSHOTARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${SNAPSHOTARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
        
#It's possible to make dist targets only using:
#cmake -DDIST_ONLY=TRUE  ..
if (DIST_ONLY)
    message("Building DIST targets ONLY (cmake -DDIST_ONLY=FALSE  .. for full build)")
    return()
endif()

#C++ is required from here on
enable_language(CXX)

set(LIB_INSTALL_DIR lib CACHE STRING "The directories where to install libraries to")
set(INCLUDE_INSTALL_DIR include CACHE STRING "The directory where to install headers to")
set(INCLUDE_INSTALL_DIR ${INCLUDE_INSTALL_DIR}/kolabxml)
set(CMAKECONFIG_INSTALL_DIR ${LIB_INSTALL_DIR}/cmake/Libkolabxml )

configure_file(libkolabxml-version.h.cmake "${CMAKE_BINARY_DIR}/libkolabxml-version.h" @ONLY)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS thread system)
if (Boost_FOUND)
    message("Found boost in ${Boost_INCLUDE_DIRS}")
endif (Boost_FOUND)

find_package(LibkolabxmlDependencies REQUIRED) # Must be after findboost

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )

execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    message(STATUS "GCC Version >= 4.7, applying unqualified lookup workaround")
    # gcc 4.7 no longer performs unqualified lookups by default, see: http://gcc.gnu.org/gcc-4.7/porting_to.html.
    # This workaround is added for xsd code, which fails otherwise to compile. -fpermissive turns the errors into warnings. 
    # It's only temporary, and should be removed once xsd is fixed.
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive" ) 
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bindings)

set( SCHEMA_DIR ${CMAKE_SOURCE_DIR}/schemas )

# Generate bindings
# WARNING: The inclusion order in SCHEMAS matters with xerces < 3.1.0. It seems without XMLUni::fgXercesHandleMultipleImports at least kolabformat-xcard.xsd MUST be before xCard.xsd,
# otherwise the compiled schema will simply lack the definitions of kolabformat-xcard.xsd (this affects xsdbin only).
set( SCHEMAS
    ${SCHEMA_DIR}/ical/kolabformat-xcal.xsd
    ${SCHEMA_DIR}/ical/iCalendar-params.xsd
    ${SCHEMA_DIR}/ical/iCalendar-props.xsd
    ${SCHEMA_DIR}/ical/iCalendar-valtypes.xsd
    ${SCHEMA_DIR}/kolabformat-xcard.xsd
    ${SCHEMA_DIR}/xCard.xsd
    ${SCHEMA_DIR}/kolabformat.xsd
)

set( SCHEMA_SOURCEFILES  
    ${CMAKE_BINARY_DIR}/bindings/kolabformat.cxx
    ${CMAKE_BINARY_DIR}/bindings/xCard.cxx
    ${CMAKE_BINARY_DIR}/bindings/kolabformat-xcal.cxx
    ${CMAKE_BINARY_DIR}/bindings/kolabformat-xcard.cxx
    ${CMAKE_BINARY_DIR}/bindings/iCalendar-params.cxx
    ${CMAKE_BINARY_DIR}/bindings/iCalendar-props.cxx
    ${CMAKE_BINARY_DIR}/bindings/iCalendar-valtypes.cxx
#     bindings/iCalendar-link-extension.cxx
#     bindings/iCalendar-bw-extensions.cxx
#     bindings/iCalendar-ms-extensions.cxx
)

#xsdcxx cxx-tree --generate-xml-schema --generate-serialization --custom-type date_time --hxx-epilogue '#include "bindings/customtypes/xml-schema-custom.hxx"' xml-schema.xsd
# --generate-inline --extern-xml-schema xml-schema.xsd
# --cxx-suffix .cpp --hxx-suffix .h   
add_custom_command(OUTPUT ${SCHEMA_SOURCEFILES}
    COMMAND ${XSDCXX} cxx-tree --generate-polymorphic --generate-serialization  --namespace-map http://kolab.org=KolabXSD --root-element icalendar --root-element vcards --root-element note --root-element configuration --output-dir ${CMAKE_BINARY_DIR}/bindings ${SCHEMAS}
    COMMENT "Generating XSD bindings"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${SCHEMAS}
    VERBATIM
    )

# Compile xsdbin if not found
if (NOT XSDBIN_FOUND)
    add_executable(xsdbin compiled/xsdbin.cxx)
    target_link_libraries(xsdbin ${XERCES_C})
    set(XSDBIN ${CMAKE_BINARY_DIR}/xsdbin)
endif ()

# Compile Schemas
add_custom_command(OUTPUT kolabformat-xcal-schema.cxx
        COMMAND ${XSDBIN} --verbose --array-name iCalendar_schema --output-dir ${CMAKE_BINARY_DIR} ${SCHEMAS}
        COMMENT "Compiling Kolab XSD schema"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${SCHEMAS} ${XSDBIN}
        VERBATIM
    )

set( SCHEMA_SOURCEFILES ${SCHEMA_SOURCEFILES} ${CMAKE_BINARY_DIR}/kolabformat-xcal-schema.cxx)

# ---------

SET_SOURCE_FILES_PROPERTIES(${SCHEMA_SOURCEFILES} PROPERTIES GENERATED 1)
ADD_CUSTOM_TARGET(generate_bindings ALL DEPENDS ${SCHEMA_SOURCEFILES})

include_directories(
    ./
    compiled
    src/containers
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${XSDCXX_INCLUDE_DIRS}
    ${XERCES_C_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)


add_subdirectory(src)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(EXPORT LibkolabxmlExport DESTINATION ${CMAKECONFIG_INSTALL_DIR} FILE LibkolabxmlTargets.cmake )

file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/${CMAKECONFIG_INSTALL_DIR} ${CMAKE_INSTALL_PREFIX} )

configure_file(${Libkolabxml_MODULE_DIR}/LibkolabxmlConfig.cmake.in ${Libkolabxml_BINARY_DIR}/LibkolabxmlConfig.cmake @ONLY )
configure_file(${Libkolabxml_MODULE_DIR}/LibkolabxmlConfigVersion.cmake.in ${Libkolabxml_BINARY_DIR}/LibkolabxmlConfigVersion.cmake @ONLY )
# Install these two files into the same directory as the generated exports-file.
install(FILES ${Libkolabxml_BINARY_DIR}/LibkolabxmlConfig.cmake ${Libkolabxml_BINARY_DIR}/LibkolabxmlConfigVersion.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR} )


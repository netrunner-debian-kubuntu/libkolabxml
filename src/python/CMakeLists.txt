# Generate Python wrapper
include_directories(../)

# Compile Python Bindings

find_package(PythonLibs REQUIRED)

if (NOT PYTHONLIBS_FOUND)
    message("python libs not found, not building python bindings")
    return()
endif()
message("found python include dirs: ${PYTHON_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH}")

set(KOLAB_SWIG_PYTHON_SOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/python_kolabformat_wrapper.cpp)
set(KOLAB_SWIG_PYTHON_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/kolabformat.py)

add_custom_command(OUTPUT ${KOLAB_SWIG_PYTHON_SOURCE_FILE} ${KOLAB_SWIG_PYTHON_HEADER_FILE}
    COMMAND ${SWIG} -v -c++ -python -o ${KOLAB_SWIG_PYTHON_SOURCE_FILE} ../kolabformat.i
    COMMENT "Generating python bindings"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ../kolabformat.i kolabxml
    VERBATIM
    )

SET_SOURCE_FILES_PROPERTIES(${KOLAB_SWIG_PYTHON_SOURCE_FILE} PROPERTIES GENERATED 1)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized" )

#${PYTHON_INCLUDE_PATH} is for backwards compatibility (el6)
include_directories(${PYTHON_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

set(PYTHON_MODULE_PREFIX "_")
python_add_module(kolabformat ${KOLAB_SWIG_PYTHON_SOURCE_FILE})

#cmake 2.6.4 fails to respect the module prefix
SET_TARGET_PROPERTIES(kolabformat PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}")

target_link_libraries(kolabformat kolabxml ${PYTHON_LIBRARIES})
configure_file(test.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(PYTHON_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/pythonbindings" CACHE STRING "Install directory for python bindings.")

install(TARGETS kolabformat LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

install( FILES
        ${KOLAB_SWIG_PYTHON_HEADER_FILE}
        DESTINATION ${PYTHON_INSTALL_DIR}
    )

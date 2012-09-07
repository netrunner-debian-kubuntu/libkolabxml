#Generate PHP wrapper
include_directories(../)

set(KOLAB_SWIG_PHP_SOURCE_FILE php_kolabformat_wrapper.cpp)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${KOLAB_SWIG_PHP_SOURCE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/kolabformat.php
        COMMAND ${SWIG} -v -c++ -php -o ${CMAKE_CURRENT_BINARY_DIR}/${KOLAB_SWIG_PHP_SOURCE_FILE}  ../kolabformat.i
        COMMENT "Generating php bindings"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ../kolabformat.i kolabxml
        VERBATIM
    )

SET_SOURCE_FILES_PROPERTIES(${KOLAB_SWIG_PHP_SOURCE_FILE} PROPERTIES GENERATED 1)
ADD_CUSTOM_TARGET(generate_php_bindings ALL DEPENDS ${KOLAB_SWIG_PHP_SOURCE_FILE})


#Compile PHP Bindings
# Since there is no php library we can't compile with -Wl,--no-undefined
if (APPLE)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flat_namespace -undefined suppress" )
endif()

find_package(PHP4 5.3 REQUIRED)

if (PHP4_FOUND)
    include_directories(${PHP4_INCLUDE_PATH})
    add_library(phpbindings SHARED ${KOLAB_SWIG_PHP_SOURCE_FILE})
    target_link_libraries(phpbindings kolabxml)
    SET_TARGET_PROPERTIES(phpbindings PROPERTIES OUTPUT_NAME "kolabformat")
    SET_TARGET_PROPERTIES(phpbindings PROPERTIES PREFIX "")

    configure_file(test.php ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

    set(PHP_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/phpbindings" CACHE STRING "Install directory for php bindings.")

    install(TARGETS phpbindings LIBRARY DESTINATION ${PHP_INSTALL_DIR})

    install( FILES
            ${CMAKE_CURRENT_BINARY_DIR}/kolabformat.php
            DESTINATION ${PHP_INSTALL_DIR}
        )
else(PHP4_FOUND)
    message(WARNING "not building php bindings because php was not found")
endif (PHP4_FOUND)

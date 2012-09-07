
#This property is not available anymore here, although present in the parent file.
SET_SOURCE_FILES_PROPERTIES(${SCHEMA_SOURCEFILES} PROPERTIES GENERATED 1)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wp,-D_FORTIFY_SOURCE=2 -O2" ) #always generate shared libraries with -fPIC, -D_FORTIFY_SOURCE=2 enables some extra checking

# Library with serialization/deserialization code and kolab-containers
add_library(kolabxml SHARED
    kolabformat.cpp
    containers/kolabcontainers.cpp
    containers/kolabnote.cpp
    containers/kolabevent.cpp
    containers/kolabtodo.cpp
    containers/kolabjournal.cpp
    containers/kolabcontact.cpp
    containers/kolabconfiguration.cpp
    containers/kolabfreebusy.cpp
    utils.cpp base64.cpp uriencode.cpp
    ../compiled/XMLParserWrapper.cpp
    ../compiled/grammar-input-stream.cxx
    ${SCHEMA_SOURCEFILES}
)
add_dependencies(kolabxml generate_bindings)
target_link_libraries(kolabxml ${XERCES_C} ${Boost_LIBRARIES} ${UUID} ${CURL})

# For the core library we can be stricter when compiling. This doesn't work with the auto generated code though.
if (${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION} VERSION_LESS 1.42)
    # We can't be as strict on with older versions of boost it seems (not sure if this is the exact version number where it breaks)
    # Since this is only for el5 compatibility we relax the rules (resulting in a couple of warning but a working build)
    #for development add here -Werror -Wfatal-errors (but don't for releases)
    set_target_properties(kolabxml PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wconversion")
else() #then normal case
    #for development add here -Werror -Wfatal-errors (but don't for releases)
    set_target_properties(kolabxml PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wconversion -Wl,--no-undefined")
endif()

set_target_properties(kolabxml PROPERTIES VERSION ${Libkolabxml_VERSION} SOVERSION ${Libkolabxml_VERSION_MAJOR})

install(TARGETS kolabxml EXPORT LibkolabxmlExport 
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)

install( FILES
    kolabformat.h
    containers/kolabevent.h
    containers/kolabevent_p.h
    containers/incidence_p.h
    containers/kolabtodo.h
    containers/kolabjournal.h
    containers/kolabcontact.h
    containers/kolabnote.h
    containers/kolabcontainers.h
    containers/kolabconfiguration.h
    containers/kolabfreebusy.h
    global_definitions.h
    DESTINATION ${INCLUDE_INSTALL_DIR})

#-----------------------SWIG--------------------
# Building of the bindings can be controlled using the switches along the lines of: cmake -DPHP_BINDINGS=TRUE -DCSHARP_BINDINGS=FALSE ..
if (SWIG_FOUND)
    if(CSHARP_BINDINGS)
        message("building csharp bindings")
        add_subdirectory(csharp)
    endif(CSHARP_BINDINGS)
    if(JAVA_BINDINGS)
        message("building java bindings")
        add_subdirectory(java)
    endif(JAVA_BINDINGS)
    if(PYTHON_BINDINGS)
        message("building python bindings")
        add_subdirectory(python)
    endif(PYTHON_BINDINGS)
    if(PHP_BINDINGS)
        message("building php bindings")
        add_subdirectory(php)
    endif(PHP_BINDINGS)
else()
    message(WARNING "Could not build SWIG bindings, because SWIG is missing.")
endif()


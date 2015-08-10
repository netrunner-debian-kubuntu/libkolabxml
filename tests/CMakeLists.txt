find_package(Qt4)
add_definitions(-DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${QT_INCLUDES} ${QT_INCLUDE_DIR} QtCore)
if (NOT APPLE)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-undefined" ) 
endif()

if(PHP_BINDINGS)
    add_test(phptest php -d enable_dl=On -d include_path='.:/usr/share/pear:${CMAKE_BINARY_DIR}/src/php' -d extension=${CMAKE_BINARY_DIR}/src/php/kolabformat.so ${CMAKE_SOURCE_DIR}/src/php/test.php --verbose)
endif()

if (QT4_FOUND)
    message("Buildings tests")
    QT4_AUTOMOC(bindingstest.cpp)
    add_executable(bindingstest bindingstest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${BINDINGSTEST_MOC})
    target_link_libraries(bindingstest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(bindingstest ${CMAKE_CURRENT_BINARY_DIR}/bindingstest)

    QT4_AUTOMOC(conversiontest.cpp)
    add_executable(conversiontest conversiontest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${CONVERSIONTEST_MOC})
    target_link_libraries(conversiontest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(conversiontest ${CMAKE_CURRENT_BINARY_DIR}/conversiontest)

    QT4_AUTOMOC(parsingtest.cpp)
    add_executable(parsingtest parsingtest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${PARSINGTEST_MOC})
    target_link_libraries(parsingtest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(parsingtest ${CMAKE_CURRENT_BINARY_DIR}/parsingtest)

    QT4_AUTOMOC(validationtest.cpp)
    add_executable(validationtest validationtest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${VALIDATIONTEST_MOC})
    target_link_libraries(validationtest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(validationtest ${CMAKE_CURRENT_BINARY_DIR}/validationtest)

    QT4_AUTOMOC(kolabconversationtest.cpp)
    add_executable(kolabconversationtest kolabconversationtest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${KOLABCONVERSATIONTEST_MOC})
    target_link_libraries(kolabconversationtest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(kolabconversationtest ${CMAKE_CURRENT_BINARY_DIR}/kolabconversationtest)
else()
    message(WARNING "Could not build tests because qt is missing")
endif()


#### Qt 4 and 5 ####
if(QT5_BUILD)
  find_package(ECM 1.1.0 REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

  find_package(Qt5Core REQUIRED)
  find_package(Qt5Test REQUIRED)

  include("../cmake/modules/ECMQt4To5Porting.cmake")
  include_directories(${QT_INCLUDES}) # TODO: Port away from this.

  if(CMAKE_VERSION VERSION_LESS 2.8.9)
    message(FATAL_ERROR "Akonadi Qt 5 build requires at least CMake version 2.8.9")
  endif()

  if (Qt5_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  endif()

  set(QT_QTTEST_LIBRARIES Qt5::Test)
else()
  set(QT_USE_IMPORTED_TARGETS TRUE) # Qt 4 only
  set(QT_MIN_VERSION 4.6.2)         # Qt 4 only

  find_package(Qt4 REQUIRED)
endif()


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

if (QT4_FOUND OR Qt5Core_FOUND)
    if (Qt5Core_FOUND)
      set(CMAKE_AUTOMOC ON)
      set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()

    message("Buildings tests")
    if (QT4_FOUND)
      QT4_AUTOMOC(bindingstest.cpp)
      QT4_AUTOMOC(conversiontest.cpp)
      QT4_AUTOMOC(parsingtest.cpp)
      QT4_AUTOMOC(validationtest.cpp)
      QT4_AUTOMOC(kolabconversationtest.cpp)
     endif()

    add_executable(bindingstest bindingstest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${BINDINGSTEST_MOC})
    target_link_libraries(bindingstest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(bindingstest ${CMAKE_CURRENT_BINARY_DIR}/bindingstest)

    add_executable(conversiontest conversiontest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${CONVERSIONTEST_MOC})
    target_link_libraries(conversiontest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(conversiontest ${CMAKE_CURRENT_BINARY_DIR}/conversiontest)

    add_executable(parsingtest parsingtest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${PARSINGTEST_MOC})
    target_link_libraries(parsingtest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(parsingtest ${CMAKE_CURRENT_BINARY_DIR}/parsingtest)

    add_executable(validationtest validationtest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${VALIDATIONTEST_MOC})
    target_link_libraries(validationtest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(validationtest ${CMAKE_CURRENT_BINARY_DIR}/validationtest)

    add_executable(kolabconversationtest kolabconversationtest.cpp ${CMAKE_CURRENT_BINARY_DIR}/${KOLABCONVERSATIONTEST_MOC})
    target_link_libraries(kolabconversationtest ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} kolabxml ${XERCES_C})
    add_test(kolabconversationtest ${CMAKE_CURRENT_BINARY_DIR}/kolabconversationtest)
else()
    message(WARNING "Could not build tests because qt is missing")
endif()

# Generate C# wrapper
include_directories(../)

# Compile CSHARP Bindings
find_program(GMCS NAMES gmcs HINTS /usr/bin/)

if (NOT GMCS)
    message("gmcs not found, not building csharp bindings")
    return()
endif()

set(KOLAB_SWIG_CSHARP_SOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/csharp_kolabformat_wrapper.cpp) #it seems we never use this file? it works at least as a dependecy.
set(KOLAB_SWIG_CSHARP_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Address.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Affiliation.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Alarm.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Attachment.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Attendee.cs
        ${CMAKE_CURRENT_BINARY_DIR}/CategoryColor.cs
        ${CMAKE_CURRENT_BINARY_DIR}/cDateTime.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Classification.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Configuration.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Contact.cs
        ${CMAKE_CURRENT_BINARY_DIR}/ContactReference.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Crypto.cs
        ${CMAKE_CURRENT_BINARY_DIR}/CustomProperty.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Cutype.cs
        ${CMAKE_CURRENT_BINARY_DIR}/DayPos.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Dictionary.cs
        ${CMAKE_CURRENT_BINARY_DIR}/DistList.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Duration.cs
        ${CMAKE_CURRENT_BINARY_DIR}/ErrorSeverity.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Email.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Event.cs
        ${CMAKE_CURRENT_BINARY_DIR}/File.cs
        ${CMAKE_CURRENT_BINARY_DIR}/FileDriver.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Freebusy.cs
        ${CMAKE_CURRENT_BINARY_DIR}/FreebusyPeriod.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Geo.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Journal.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Key.cs
        ${CMAKE_CURRENT_BINARY_DIR}/kolabformatPINVOKE.cs
        ${CMAKE_CURRENT_BINARY_DIR}/NameComponents.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Note.cs
        ${CMAKE_CURRENT_BINARY_DIR}/PartStatus.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Period.cs
        ${CMAKE_CURRENT_BINARY_DIR}/RecurrenceRule.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Related.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Relation.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Relative.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Role.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Snippet.cs
        ${CMAKE_CURRENT_BINARY_DIR}/SnippetsCollection.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Status.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Telephone.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Todo.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Url.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectoraddress.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectoraffiliation.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectoralarm.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorattachment.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorattendee.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorcategorycolor.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorcontactref.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorcs.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectordatetime.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectordaypos.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectoremail.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorevent.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorfreebusyperiod.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorgeo.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectori.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorkey.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorperiod.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorrelated.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectors.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorsnippet.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectortelephone.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectortodo.cs
        ${CMAKE_CURRENT_BINARY_DIR}/vectorurl.cs
        ${CMAKE_CURRENT_BINARY_DIR}/Weekday.cs
    )
set(KOLAB_SWIG_CSHARP_DLL_FILE ${CMAKE_CURRENT_BINARY_DIR}/kolabformat.dll)
set(KOLAB_SWIG_CSHARP_TEST_FILE ${CMAKE_CURRENT_BINARY_DIR}/test.exe)

add_custom_command(OUTPUT ${KOLAB_SWIG_CSHARP_SOURCE_FILE} ${KOLAB_SWIG_CSHARP_FILES}
        COMMAND ${SWIG} -v -c++ -csharp -o ${KOLAB_SWIG_CSHARP_SOURCE_FILE}  ../kolabformat.i
        COMMENT "Generating C# bindings"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ../kolabformat.i kolabxml
        VERBATIM
    )

SET_SOURCE_FILES_PROPERTIES(${KOLAB_SWIG_CSHARP_SOURCE_FILE} PROPERTIES GENERATED 1)
SET_SOURCE_FILES_PROPERTIES(${KOLAB_SWIG_CSHARP_FILES} PROPERTIES GENERATED 1)

configure_file(test.cs ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_custom_command(OUTPUT ${KOLAB_SWIG_CSHARP_DLL_FILE}
        COMMAND ${GMCS} -target:library "${CMAKE_CURRENT_BINARY_DIR}/kolabformat.cs" ${KOLAB_SWIG_CSHARP_FILES}
        COMMENT "Compiling C# library"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS kolabxml ${KOLAB_SWIG_CSHARP_SOURCE_FILE} ${KOLAB_SWIG_CSHARP_FILES}
        VERBATIM
    )

add_custom_command(OUTPUT ${KOLAB_SWIG_CSHARP_TEST_FILE}
        COMMAND ${GMCS} -reference:${KOLAB_SWIG_CSHARP_DLL_FILE} ${CMAKE_CURRENT_BINARY_DIR}/test.cs
        COMMENT "Compiling C# test.exe"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS kolabxml ${KOLAB_SWIG_CSHARP_SOURCE_FILE} ${KOLAB_SWIG_CSHARP_DLL_FILE}
        VERBATIM
    )

SET_SOURCE_FILES_PROPERTIES(${KOLAB_SWIG_CSHARP_DLL_FILE} PROPERTIES GENERATED 1)
SET_SOURCE_FILES_PROPERTIES(${KOLAB_SWIG_CSHARP_TEST_FILE} PROPERTIES GENERATED 1)
ADD_CUSTOM_TARGET(generate_csharp_bindings ALL DEPENDS ${KOLAB_SWIG_CSHARP_DLL_FILE} ${KOLAB_SWIG_CSHARP_TEST_FILE})

set(CSHARP_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/csharpbindings" CACHE STRING "Install directory for csharp bindings.")

#not sure about the LIBRARY here, but might work
install(FILES ${KOLAB_SWIG_CSHARP_DLL_FILE} DESTINATION ${CSHARP_INSTALL_DIR})
install(FILES ${KOLAB_SWIG_CSHARP_TEST_FILE} DESTINATION ${CSHARP_INSTALL_DIR})


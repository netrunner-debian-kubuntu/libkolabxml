# Generate Java wrapper
include_directories(../)

set(KOLAB_SWIG_JAVA_SOURCE_FILE java_kolabformat_wrapper.cpp)
set(KOLAB_SWIG_JAVA_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Address.java
        ${CMAKE_CURRENT_BINARY_DIR}/Affiliation.java
        ${CMAKE_CURRENT_BINARY_DIR}/Alarm.java
        ${CMAKE_CURRENT_BINARY_DIR}/Attachment.java
        ${CMAKE_CURRENT_BINARY_DIR}/Attendee.java
        ${CMAKE_CURRENT_BINARY_DIR}/CategoryColor.java
        ${CMAKE_CURRENT_BINARY_DIR}/cDateTime.java
        ${CMAKE_CURRENT_BINARY_DIR}/Classification.java
        ${CMAKE_CURRENT_BINARY_DIR}/Configuration.java
        ${CMAKE_CURRENT_BINARY_DIR}/Contact.java
        ${CMAKE_CURRENT_BINARY_DIR}/ContactReference.java
        ${CMAKE_CURRENT_BINARY_DIR}/Crypto.java
        ${CMAKE_CURRENT_BINARY_DIR}/CustomProperty.java
        ${CMAKE_CURRENT_BINARY_DIR}/Cutype.java
        ${CMAKE_CURRENT_BINARY_DIR}/DayPos.java
        ${CMAKE_CURRENT_BINARY_DIR}/Dictionary.java
        ${CMAKE_CURRENT_BINARY_DIR}/DistList.java
        ${CMAKE_CURRENT_BINARY_DIR}/Duration.java
        ${CMAKE_CURRENT_BINARY_DIR}/ErrorSeverity.java
        ${CMAKE_CURRENT_BINARY_DIR}/Event.java
        ${CMAKE_CURRENT_BINARY_DIR}/Geo.java
        ${CMAKE_CURRENT_BINARY_DIR}/Journal.java
        ${CMAKE_CURRENT_BINARY_DIR}/Key.java
        ${CMAKE_CURRENT_BINARY_DIR}/kolabformat.java
        ${CMAKE_CURRENT_BINARY_DIR}/kolabformatJNI.java
        ${CMAKE_CURRENT_BINARY_DIR}/Makefile
        ${CMAKE_CURRENT_BINARY_DIR}/NameComponents.java
        ${CMAKE_CURRENT_BINARY_DIR}/Note.java
        ${CMAKE_CURRENT_BINARY_DIR}/PartStatus.java
        ${CMAKE_CURRENT_BINARY_DIR}/RecurrenceRule.java
        ${CMAKE_CURRENT_BINARY_DIR}/Related.java
        ${CMAKE_CURRENT_BINARY_DIR}/Relative.java
        ${CMAKE_CURRENT_BINARY_DIR}/Role.java
        ${CMAKE_CURRENT_BINARY_DIR}/Status.java
        ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_Kolab__Freebusy.java
        ${CMAKE_CURRENT_BINARY_DIR}/Telephone.java
        ${CMAKE_CURRENT_BINARY_DIR}/Todo.java
        ${CMAKE_CURRENT_BINARY_DIR}/Url.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectoraddress.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectoraffiliation.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectoralarm.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorattachment.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorattendee.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorcategorycolor.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorcontactref.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorcs.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectordatetime.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectordaypos.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorgeo.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectori.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorkey.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorrelated.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectors.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectortelephone.java
        ${CMAKE_CURRENT_BINARY_DIR}/vectorurl.java
        ${CMAKE_CURRENT_BINARY_DIR}/Weekday.java
    )

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${KOLAB_SWIG_JAVA_SOURCE_FILE} ${KOLAB_SWIG_JAVA_FILES}
    COMMAND ${SWIG} -v -c++ -java -o ${CMAKE_CURRENT_BINARY_DIR}/${KOLAB_SWIG_JAVA_SOURCE_FILE}  ../kolabformat.i
    COMMENT "Generating Java bindings"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ../kolabformat.i kolabxml
    VERBATIM
    )

SET_SOURCE_FILES_PROPERTIES(${KOLAB_SWIG_JAVA_SOURCE_FILE} PROPERTIES GENERATED 1)

ADD_CUSTOM_TARGET(generate_JAVA_bindings ALL DEPENDS ${KOLAB_SWIG_JAVA_SOURCE_FILE})

# Compile JAVA Bindings

set(JAVA_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/javabindings" CACHE STRING "Install directory for java bindings.")

install( FILES
        ${KOLAB_SWIG_JAVA_FILES}
        DESTINATION ${JAVA_INSTALL_DIR}
    )

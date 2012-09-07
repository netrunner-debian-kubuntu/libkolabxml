/* kolabformat.i */
%module kolabformat
%{

    /* This macro ensures that return vectors remain a vector also in python and are not converted to tuples */
    #define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS 

    #include "global_definitions.h"
    #include "kolabformat.h"
    #include "containers/kolabcontainers.h"
    #include "containers/kolabevent.h"
    #include "containers/kolabtodo.h"
    #include "containers/kolabjournal.h"
    #include "containers/kolabcontact.h"
    #include "containers/kolabnote.h"
    #include "containers/kolabconfiguration.h"
%}

%include "std_string.i"
%include "std_vector.i"

namespace std {
    %template(vectori) vector<int>;
    %template(vectors) vector<string>;
    %template(vectordaypos) vector<Kolab::DayPos>;
    %template(vectorcs) vector<Kolab::CustomProperty>;
    %template(vectoraddress) vector<Kolab::Address>;
    %template(vectoraffiliation) vector<Kolab::Affiliation>;
    %template(vectoralarm) vector<Kolab::Alarm>;
    %template(vectorattachment) vector<Kolab::Attachment>;
    %template(vectorattendee) vector<Kolab::Attendee>;
    %template(vectorcontactref) vector<Kolab::ContactReference>;
    %template(vectorgeo) vector<Kolab::Geo>;
    %template(vectorrelated) vector<Kolab::Related>;
    %template(vectortelephone) vector<Kolab::Telephone>;
    %template(vectordatetime) vector<Kolab::cDateTime>;
    %template(vectorurl) vector<Kolab::Url>;
    %template(vectorkey) vector<Kolab::Key>;
    %template(vectorcategorycolor) vector<Kolab::CategoryColor>;
};

%include "global_definitions.h"
%include "kolabformat.h"
%include "containers/kolabcontainers.h"
%include "containers/kolabevent.h"
%include "containers/kolabtodo.h"
%include "containers/kolabjournal.h"
%include "containers/kolabcontact.h"
%include "containers/kolabnote.h"
%include "containers/kolabconfiguration.h"

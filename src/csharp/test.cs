using System;

class MyEvent {

    static void Main() {
        Event myEvent = new Event();

        ContactReference myContactReference = new ContactReference("john.doe@example.org");
        myContactReference.setName("John Doe");

        myEvent.setOrganizer(myContactReference);

    }
}

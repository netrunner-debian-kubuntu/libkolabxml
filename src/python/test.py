import kolabformat
import unittest

class TestKolabformat(unittest.TestCase):

    def test_event_basic(self):
        e = kolabformat.Event()
        e.setSummary("test")

        start = kolabformat.cDateTime(2014,7,1, 12,30,0)
        start.setTimezone("Europe/London")
        e.setStart(start)

        ex = e.exceptionDates()
        ex.size()
        ex.push_back(kolabformat.cDateTime(2014,7,5))
        ex.size()
        e.exceptionDates().size()
        e.setExceptionDates(ex)
        e.exceptionDates().size()

        xml = kolabformat.writeEvent(e)
        e1 = kolabformat.readEvent(xml, False)
        xml1 = kolabformat.writeEvent(e1)
        self.assertEqual(xml, xml1)

    def test_event_delegated(self):
        e = kolabformat.Event()
        e.setSummary("test")
        e.setStart(kolabformat.cDateTime(2014,7,1, 12,30,0))

        att1 = kolabformat.Attendee(kolabformat.ContactReference("john@doe.org"))
        att1.setRole(kolabformat.NonParticipant)
        att1.setPartStat(kolabformat.PartDelegated)
        att1.setDelegatedTo([kolabformat.ContactReference("jane@doe.org")])

        att2 = kolabformat.Attendee(kolabformat.ContactReference("jane@doe.org"))
        att2.setRole(kolabformat.Required)
        att2.setPartStat(kolabformat.PartNeedsAction)
        att2.setDelegatedFrom([kolabformat.ContactReference("john@doe.org")])

        e.setAttendees([att1, att2])
        self.assertEqual(len(att1.delegatedTo()), 1)
        self.assertEqual(len(att2.delegatedFrom()), 1)

        xml = kolabformat.writeEvent(e)
        e1 = kolabformat.readEvent(xml, False)
        # print xml

        attendees_ = e1.attendees()
        self.assertEqual(len(attendees_), 2)
        self.assertEqual(len(attendees_[0].delegatedTo()), 1)
        self.assertEqual(len(attendees_[1].delegatedFrom()), 1)

        delegatees = attendees_[0].delegatedTo()
        self.assertEqual(delegatees[0].email(), "jane@doe.org")

    def test_todo_partstat(self):
        t = kolabformat.Todo()
        t.setSummary("test")
        t.setPercentComplete(40)

        att = kolabformat.Attendee(kolabformat.ContactReference("john@doe.org"))
        att.setRole(kolabformat.Required)
        att.setPartStat(kolabformat.PartInProcess)
        t.setAttendees([att])

        xml = kolabformat.writeTodo(t)
        self.assertTrue("<text>IN-PROCESS</text>" in xml)

        t1 = kolabformat.readTodo(xml, False)
        attendees = t1.attendees()
        self.assertEqual(attendees[0].partStat(), kolabformat.PartInProcess)


if __name__ == '__main__':
    unittest.main()
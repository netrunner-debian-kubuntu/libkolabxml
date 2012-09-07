import kolabformat
e = kolabformat.Event()

ex = e.exceptionDates()
ex.size()
ex.push_back(kolabformat.cDateTime(1,1,1))
ex.size()
e.exceptionDates().size()
e.setExceptionDates(ex)
e.exceptionDates().size()

string = kolabformat.writeEvent(e);
print string;
e1 = kolabformat.readEvent(string, False);
string = kolabformat.writeEvent(e1);
print string;
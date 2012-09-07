<?php
//run using "php -d enable_dl=On -d extension=./kolabformat.so  test.php"

include("kolabformat.php");

function assertequal($got, $expect, $name) {
	if ($got == $expect) {
		print "OK - $name\n";
		return true;
	}
	else {
		print "FAIL - $name\n";
		print "-- Expected " . var_export($expect, true) . ", got " . var_export($got, true) . " --\n";
		return false;
	}
}

function assertcontains($haystack, $needle, $name) {
	// remove whitespace
	$haystack = preg_replace('/\n\s*/ims', '', $haystack);
	$needle = preg_replace('/\n\s*/ims', '', $needle);

	return assertequal(substr(strstr($haystack, $needle), 0, strlen($needle)), $needle, $name);
}

function asserttrue($arg, $name) {
	return assertequal($arg, true, $name);
}

function assertfalse($arg, $name) {
	return assertequal($arg, false, $name);
}

function array2vector($arr) {
	$vec = new vectors;
	foreach ((array)$arr as $val)
		$vec->push($val);
	return $vec;
}


/////// Test Event

$e = new Event();
$e->setCreated(new cDateTime(2012,3,14, 9,5,30, true));
$e->setStart(new cDateTime(2012,7,31));
$d = new cDateTime(2009,10,12);
assertequal($e->exceptionDates()->size(), 0, "Event::exceptionDates()");
$e->addExceptionDate($d);
assertequal($e->exceptionDates()->size(), 1, "Event::addExceptionDate()");

$r = new RecurrenceRule();
$r->setFrequency(RecurrenceRule::Minutely);

$bysec = new vectori();
$bysec->push(1);
$bysec->push(3);

$r->setBySecond($bysec);
$e->setRecurrenceRule($r);

$va = new vectoralarm;
$a1 = new Alarm("Display alarm");
$a1->setRelativeStart(new Duration(0, 6, 30, 0, true), kolabformat::Start);
$va->push($a1);
$acr = new vectorcontactref;
$acr->push(new ContactReference("alarms@kolab.org"));
$a2 = new Alarm("Email alarm", "Alarm description", $acr);
$a2->setStart(new cDateTime(2012,7,30, 20,30,0, true));
$va->push($a2);
$e->setAlarms($va);

$xml = kolabformat::writeEvent($e);
#print $xml;
assertcontains($xml, '<dtstart><date>2012-07-31</date></dtstart>', "Event::setStart() with date only");
assertcontains($xml, '<created><date-time>2012-03-14T09:05:30Z</date-time></created>', "Event::setCreated() with date-time");
assertcontains($xml, '<rrule><recur><freq>MINUTELY</freq>', "RecurrenceRule::setFrequency()");
assertcontains($xml, '<rrule><recur><freq>MINUTELY</freq><bysecond>1</bysecond><bysecond>3</bysecond></recur></rrule>', "RecurrenceRule::setBySecond()");
assertcontains($xml, '<exdate><date>2009-10-12</date></exdate>', "Event Recurrence Exceptions");
assertcontains($xml, '<duration>-PT6H30M</duration>', "Alarm::setRelativeStart(Duration)");
assertcontains($xml, '<trigger><date-time>2012-07-30T20:30:00Z</date-time>', "Alarm::setStart()");
assertcontains($xml, '<trigger><parameters><related><text>START</text>', "Alarm relative to Start");
assertcontains($xml, '<valarm><properties><action><text>EMAIL</text>', "Email alarm");
assertcontains($xml, '<attendee><cal-address>mailto:%3Calarms%40kolab%2Eorg%3E</cal-address>', "Email alarm attendee");

$e1 = kolabformat::readEvent($xml, false);
assertequal($xml, kolabformat::writeEvent($e1), "kolabformat::readEvent() => kolabformat::writeEvent()");

$s = $e1->start();
assertequal(sprintf('%d-%d-%d', $s->year(), $s->month(), $s->day()), '2012-7-31', "Event::start()");

$r = $e1->recurrenceRule();
assertequal($r->frequency(), RecurrenceRule::Minutely, "RecurrenceRule::frequency()");
assertequal($r->bysecond()->size(), 2, "RecurrenceRule::bysecond()");
assertequal($e1->exceptionDates()->size(), 1, "Read Event::exceptionDates()");

$va = $e1->alarms();
assertequal($va->size(), 2, "Event::alarms()");

$rr = new RecurrenceRule();
$rr->setFrequency(RecurrenceRule::Monthly);
$byday = new vectordaypos;
$byday->push(new DayPos(2, kolabformat::Monday));
$byday->push(new DayPos(-1, kolabformat::Friday));
$rr->setByday($byday);
$e1->setRecurrenceRule($rr);
$xml = kolabformat::writeEvent($e1);
#print $xml;
assertcontains($xml, '<byday>2MO</byday><byday>-1FR</byday>', "Recurrence by weekday");



/////// Test Contact

$c = new Contact;
$c->setName("Contact-FN");

$nc = new NameComponents;
$nc->setSurnames(array2vector("Surname"));
$nc->setGiven(array2vector("Given"));
$nc->setAdditional(array2vector(array("Middle1", "Middle2")));
$nc->setPrefixes(array2vector("Prefix"));
#$nc->setSuffixes(new vectors);

assertcontains($nc->surnames()->size(),    1, "NameComponents::setSurnames()");
assertcontains($nc->given()->size(),       1, "NameComponents::setGiven()");
assertcontains($nc->additional()->size(),  2, "NameComponents::setAdditional()");
assertcontains($nc->prefixes()->size(),    1, "NameComponents::setPrefixes()");
assertcontains($nc->suffixes()->size(),    0, "NameComponents::suffixes()");

$c->setNameComponents($nc);
$c->setTitles(array2vector("MyProfession"));

$pic = "R0lGODlhEgASAIAAAMDAwAAAACH5BAEAAAAALAAAAAASABIAQAIPhI+py+0Po5y02ouz3pwXADs=";
$c->setPhoto(base64_decode($pic), 'image/gif');
$geo = new vectorgeo;
$geo->push(new Geo(46.952585, 7.43766));
$c->setGPSpos($geo);

$bd = new cDateTime(1980,8,1);
$c->setBDay($bd);

$vcs = new vectorcs;
$vcs->push(new CustomProperty("initials", "KF"));
$c->setCustomProperties($vcs);

$xml = kolabformat::writeContact($c);
#print $xml;
assertcontains($xml, "<uid><uri>urn:uuid:", "Generate Contact UID as urn::uuid");
assertcontains($xml, "<n><surname>Surname</surname><given>Given</given><additional>Middle1</additional><additional>Middle2</additional><prefix>Prefix</prefix></n>", "Contact::setNameComponents()");
assertcontains($xml, "<title><text>MyProfession</text></title>", "Contact::setTitles()");
assertcontains($xml, "<photo><uri>data:image/gif;base64,$pic</uri></photo>", "Contact::setPhoto()");
assertcontains($xml, "<bday><date>19800801</date></bday>", "Contact::setBDay()");
assertcontains($xml, "<geo><uri>geo:46.952585,7.43766</uri></geo>", "Contact::setGPSpos()");

$c1 = kolabformat::readContact($xml, false);
assertequal($xml, kolabformat::writeContact($c1), "kolabformat::readContact() => kolabformat::writeContact()");
assertequal($c1->photoMimetype(), "image/gif", "Contact::photoMimetype()");
assertequal($c1->uid(), kolabformat::getSerializedUID(), "kolabformat::getSerializedUID()");
$lm = $c1->lastModified();
assertequal(sprintf('%d-%02d-%02d', $lm->year(), $lm->month(), $lm->day()), gmdate('Y-m-d'), "Contact::lastModified()");

$c1->setBDay(new cDateTime);
$xml = kolabformat::writeContact($c);
assertfalse(strpos($xml, '<bday><date>'), "Unset BDay with empty cDateTime");


/////// Test DistList

$dl = new DistList;
$dl->setName("DalistÄÖŸ");

$m = new vectorcontactref;
$a = new ContactReference(ContactReference::EmailReference, "a@localhost", "Member-A");
$m->push($a);

$b = new ContactReference(ContactReference::UidReference, "x-member-b-fff");
$b->setName("Member-B");
$m->push($b);

#$c = new ContactReference(ContactReference::EmailAndUidReference, "c@localhost", "dddaab06-0000-0000-eeb5-cc64ff7f0000");
#$c->setName("Member-C");
#$m->push($c);

assertequal($m->size(), 2, "vectorcontactref::size()");
$dl->setMembers($m);

$xml = kolabformat::writeDistlist($dl);
#print $xml;
assertcontains($xml, '<fn><text>DalistÄÖŸ</text></fn>', "kolabformat::writeDistlist(): FN (UTF-8)");
assertcontains($xml, '<uri>mailto:Member%2DA%3Ca%40localhost%3E</uri>', "kolabformat::writeDistlist(): mailto uri");
assertcontains($xml, '<member><uri>urn:uuid:x-member-b-fff</uri>', "kolabformat::writeDistlist(): member urn::uuid");



/////// Test Configuration.Dictionary

$d = new Dictionary('de');
$d->setEntries(array2vector(array('Kolab','Roundcube','libkolabxml')));
$c = new Configuration($d);

$xml = kolabformat::writeConfiguration($c);
#print $xml;
$c2 = kolabformat::readConfiguration($xml, false);
$d2 = $c2->dictionary();

assertequal($d2->language(), 'de', "Configuration.Dictionary::language");
assertequal($d2->entries()->size(), 3, "Dictionary::entries()");

$d = new Dictionary('en');
$d->setEntries(array2vector(array('Kolab','Roundcube','libkolabxml')));
$c = new Configuration($d);

$xml = kolabformat::writeConfiguration($c);
#print $xml;
$c3 = kolabformat::readConfiguration($xml, false);

asserttrue($c2->uid() != $c3->uid(), "Generate different UIDs for configuration objects");

?>




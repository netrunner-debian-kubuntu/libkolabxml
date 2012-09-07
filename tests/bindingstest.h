#ifndef BINDINGSTEST_H
#define BINDINGSTEST_H

#include <QtCore/QObject>
#include <QtTest/QtTest>

/*
 * The test are roundtrip tests, which simply write an object out and read it again. The two objects are then compared for equality.
 * This assumes that containers are working (comparison operators and adding/removing values).
 * 
 * Testing it properly would mean to add loads of testfiles in text so we could:
 * serialize => compare to text representation
 * deserialize => check values
 * 
 * If we would do this ideally for every property on every type that would result in a lot of work.
 *
 */ 

class BindingsTest : public QObject
{
  Q_OBJECT
  private slots:

    //Kolabformat
    void categorycolorConfigurationCompletness();
    void dictionaryConfigurationCompletness();
    void noteCompletness();
    void eventCompletness();
    void eventDuration();
    void todoCompletness();
    void dueDateDateOnly();
    void journalCompletness();
    void freebusyCompletness();
    
    void contactCompletness();
    void distlistCompletness();

    void generateTimestampIfEmpty();

    void versionTest();
    void errorTest();
    void errorRecoveryTest();

    void BenchmarkRoundtripKolab();
    void BenchmarkRoundtrip();

    void preserveLatin1();
    void preserveUnicode();

};

#endif
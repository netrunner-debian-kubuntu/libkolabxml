
#ifndef CONVERSIONTEST_H
#define CONVERSIONTEST_H

#include <QObject>

class ConversionTest : public QObject
{
  Q_OBJECT
  private slots:
    void dateComparatorTest();

    void durationParserTest_data();
    void durationParserTest();
    
    void durationSerializerTest_data();
    void durationSerializerTest();
    
    void dayPosParserTest_data();
    void dayPosParserTest();
    
    void dayPosSerializerTest_data();
    void dayPosSerializerTest();
    
    void xcardDateParserTest_data();
    void xcardDateParserTest();
    
    void xCardSerializerTest_data();
    void xCardSerializerTest();
    
    void uriInlineEncodingTest();
    
    void mailtoUriEncodingTest_data();
    void mailtoUriEncodingTest();
    void mailtoUriDecodingTest();
    
    void urnTest();
    
    void contactReferenceTest();
    
    void geoUriTest();
    
    void threadLocalTest();
    
    void uuidGeneratorTest();
    void uuidGeneratorTest2();
};

#endif // CONVERSIONTEST_H

INCLUDEPATH += $$PWD

equals( QT_MAJOR_VERSION, 5 ): greaterThan( QT_MINOR_VERSION, 10 )
{
    QT += datavisualization
    QT += widgets

    HEADERS += $$PWD/customscatter.h
    SOURCES += $$PWD/customscatter.cpp

    message( "CustomScatter class appended" )
}

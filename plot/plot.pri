QT += datavisualization
QT += widgets

INCLUDEPATH += $$PWD

equals( QT_MAJOR_VERSION, 5 ): greaterThan( QT_MINOR_VERSION, 10 )
{
        HEADERS += $$PWD/customscatter.h
        SOURCES += $$PWD/customscatter.cpp

        message( "CustomScatter class appended" )
}

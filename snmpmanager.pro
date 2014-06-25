#-------------------------------------------------
#
# Project created by QtCreator 2014-06-23T12:46:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snmpmanager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutDialog.cpp \
    manageDialog.cpp

HEADERS  += mainwindow.h \
    aboutDialog.h \
    manageDialog.h

FORMS    += mainwindow.ui \
    aboutDialog.ui \
    manageDialog.ui

OTHER_FILES += \
    snmpmanager.pro.user

unix{
LIBS += "$$PWD/snmp++/lib/libsnmp++.a" -lssl -lcrypto
}

win32{
LIBS += -lsnmp++ -lwsock32 -LC:/OpenSSL-Win32 -leay32 -lssleay32
}

INCLUDEPATH += snmp++/include snmp++/include/snmp_pp

RESOURCES += resources.qrc

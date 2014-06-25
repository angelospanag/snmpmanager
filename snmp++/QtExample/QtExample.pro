
CONFIG += qt thread warn_on debug
INCLUDEPATH += ../include
DEFINES += BSDUNIX

unix {
  TEMPLATE = app
}
win32 {
  TEMPLATE = vcapp
}

# Activate either libtomcrypt or libdes:
unix {
#  LIBS += -lsnmp++ -L../lib -ldes -L../../libdes
  LIBS += -lsnmp++ -L../lib -ltomcrypt -L../../crypt
}
win32 {
#  LIBS += ../libsnmp_pp.lib ../../libdes/libdes.lib
  LIBS += ../libsnmp_pp.lib ../../crypt/libtomcrypt.lib
INCLUDEPATH += snmp++/include snmp++/include/snmp_pp
LIBS += -lsnmp++ -lwsock32 -LC:/OpenSSL-Win32 -leay32 -lssleay32

}

# Input
HEADERS += MainWindow.h
FORMS3 += MainWindowPrivate.ui Properties.ui DialogAddUser.ui
SOURCES += main.cpp MainWindow.cpp
QT +=  qt3support 
CONFIG += uic3

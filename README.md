SNMP Manager
===========

#Description

SNMP Manager is a graphical network management application that makes use of the SNMP protocol.

The graphical user interface was build using Qt: http://qt-project.org/

SNMP implementation in C++ using the SNMP++ API: http://www.agentpp.com/snmp_pp3_x/snmp_pp3_x.html

#Installation

##Ubuntu

###Dependencies

SNMP Manager is build using Qt and uses OpenSSL for some of its cryptographic functionality.
Install all the required dependencies:

```
sudo apt-get install qt-sdk libssl-dev
```

###Building the SNMP++ library

From the root folder of the project, execute the following commands to build the SNMP++ library:

```
cd snmp++/src/
make -f Makefile.linux
```
The library should now be located in snmp++/lib

###Building SNMP Manager
You can use Qt Creator IDE or simply execute the following commands from the root folder of the project:

```
qmake snmpmanager.pro
qmake
make
```
All done!

##Windows

TODO


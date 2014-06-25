#ifndef MANAGEDIALOG_H
#define MANAGEDIALOG_H

#include <QDialog>
#include <QTimer>
#include "snmp++/include/snmp_pp/snmp_pp.h"
#include "snmp++/include/snmp_pp/address.h"
#include "snmp++/include/snmp_pp/oid.h"
#include "ui_manageDialog.h"
#include <iostream>

class manageDialog : public QDialog
{
    Q_OBJECT

public:
    manageDialog(QWidget *parent = 0);
    QString getAddress();
    unsigned short int getPort();
    QString getVersion();
    QString getCommunity();
    QString getsecurityName();
    QString getsecurityLevel();
    QString getauthProtocol();
    QString getauthPassword();
    QString getprivProtocol();
    QString getprivPassword();
    QString getContextName();
    QString getContextEngineID();


private:
    Ui_manage ui;
    QString snmpget_value, if_num;
    long prev_ifInOctets, prev_ifOutOctets, prev_timeticks, prev_ifInUcastPkts, prev_ifInNUcastPkts, prev_ifOutUcastPkts, prev_ifOutNUcastPkts;
    int myrow, mycolumn;
    unsigned short int manage_port;
    QString manage_address, manage_version, manage_community, manage_privPassword, manage_authPassword, manage_securityName, manage_ContextName, manage_ContextEngineID, manage_securityLevel, manage_authProtocol, manage_privProtocol;

public slots:
    void snmpWalk();
    void snmpGet(QString);

    void snmpGet_sysDescr();
    void snmpGet_sysObjectID();
    void snmpGet_sysUpTime();
    void snmpGet_sysContact();
    void snmpGet_sysName();
    void snmpGet_sysLocation();

    void setAddress(QString);
    void setPort(unsigned short int);
    void setVersion(QString);
    void setCommunity(QString);
    void setprivPassword(QString);
    void setauthPassword(QString);
    void setsecurityName(QString);
    void setContextName(QString);
    void setContextEngineID(QString);
    void setsecurityLevel(QString);
    void setauthProtocol(QString);
    void setprivProtocol(QString);
    QString vb_to_str(SmiUINT32);

    void on_change_cell_on_table(int, int);
    void change_poll_sec();
    void statistics();
};

#endif // MANAGEDIALOG_H

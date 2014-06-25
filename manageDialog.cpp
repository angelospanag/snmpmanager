#include "manageDialog.h"

using namespace std;
#define BULK_MAX 10
QTimer *timer = new QTimer();
manageDialog::manageDialog(QWidget *parent)
   : QDialog(parent, Qt::Sheet)
 {
    if (timer->isActive()==true){ timer->stop();}
    ui.setupUi(this); 
    connect(ui.tableWidget, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(on_change_cell_on_table(int, int)));
    connect(ui.button_refresh,SIGNAL(clicked()), this, SLOT(snmpWalk()));
    connect(ui.pushButton_sysDescr,SIGNAL(clicked()), this, SLOT(snmpGet_sysDescr()));
    connect(ui.pushButton_sysObjectID,SIGNAL(clicked()), this, SLOT(snmpGet_sysObjectID()));
    connect(ui.pushButton_sysUpTime,SIGNAL(clicked()), this, SLOT(snmpGet_sysUpTime()));
    connect(ui.pushButton_sysContact,SIGNAL(clicked()), this, SLOT(snmpGet_sysContact()));
    connect(ui.pushButton_sysName,SIGNAL(clicked()), this, SLOT(snmpGet_sysName()));
    connect(ui.pushButton_sysLocation,SIGNAL(clicked()), this, SLOT(snmpGet_sysLocation()));
    connect(timer, SIGNAL(timeout()), this, SLOT(statistics()));
    connect(ui.poll_button, SIGNAL(clicked()), this, SLOT(change_poll_sec()));
}

void manageDialog::change_poll_sec()
{
    if (timer->isActive()==true){ timer->stop();}
    timer->start(ui.poll_ms->value());
}
void manageDialog::setAddress(QString address){
    manage_address=address;
}

QString manageDialog::getAddress(){
    return manage_address;
}

void manageDialog::setPort(unsigned short int port){
    manage_port=port;
}

unsigned short int manageDialog::getPort(){
    return manage_port;
}

void manageDialog::setVersion(QString version){
    manage_version=version;
}

QString manageDialog::getVersion(){
    return manage_version;
}

void manageDialog::setCommunity(QString community){
    manage_community=community;
}

QString manageDialog::getCommunity(){
    return manage_community;
}


QString manageDialog::getprivPassword(){
    return manage_privPassword;
}

QString manageDialog::getauthPassword(){
    return manage_authPassword;
}

QString manageDialog::getsecurityName(){
    return manage_securityName;
}

QString manageDialog::getsecurityLevel(){
    return manage_securityLevel;
}

QString manageDialog::getContextName(){
    return manage_ContextName;
}

QString manageDialog::getContextEngineID(){
    return manage_ContextEngineID;
}

QString manageDialog::getauthProtocol(){
    return manage_authProtocol;
}

QString manageDialog::getprivProtocol(){
    return manage_privProtocol;
}

void manageDialog::setprivPassword(QString privPassword){
    manage_privPassword=privPassword;
}

void manageDialog::setauthPassword(QString authPassword){
    manage_authPassword=authPassword;
}

void manageDialog::setsecurityName(QString securityName){
    manage_securityName=securityName;
}

void manageDialog::setsecurityLevel(QString securityLevel){
    manage_securityLevel=securityLevel;
}

void manageDialog::setContextName(QString ContextName){
    manage_ContextName=ContextName;
}

void manageDialog::setContextEngineID(QString ContextEngineID){
    manage_ContextEngineID=ContextEngineID;
}

void manageDialog::setauthProtocol(QString authProtocol){
    manage_authProtocol=authProtocol;
}

void manageDialog::setprivProtocol(QString privProtocol){
    manage_privProtocol=privProtocol;
}


void manageDialog::snmpGet_sysDescr(){
    if (timer->isActive()==true){ timer->stop();}
    ui.objId->setText("1.3.6.1.2.1.1.1.0");
    snmpGet("1.3.6.1.2.1.1.1.0");
    ui.label_value->setText(snmpget_value);
    //ui.label_syntax->setText(item_clicked_3->text());
}
void manageDialog::snmpGet_sysObjectID(){
    if (timer->isActive()==true){ timer->stop();}
    ui.objId->setText("1.3.6.1.2.1.1.2.0");
    snmpGet("1.3.6.1.2.1.1.2.0");
    ui.label_value->setText(snmpget_value);
}
void manageDialog::snmpGet_sysUpTime(){
    if (timer->isActive()==true){ timer->stop();}
    ui.objId->setText("1.3.6.1.2.1.1.3.0");
    snmpGet("1.3.6.1.2.1.1.3.0");
    ui.label_value->setText(snmpget_value);
}
void manageDialog::snmpGet_sysContact(){
    if (timer->isActive()==true){ timer->stop();}
    ui.objId->setText("1.3.6.1.2.1.1.4.0");
    snmpGet("1.3.6.1.2.1.1.4.0");
    ui.label_value->setText(snmpget_value);
}
void manageDialog::snmpGet_sysName(){
    if (timer->isActive()==true){ timer->stop();}
    ui.objId->setText("1.3.6.1.2.1.1.5.0");
    snmpGet("1.3.6.1.2.1.1.5.0");
    ui.label_value->setText(snmpget_value);
}
void manageDialog::snmpGet_sysLocation(){
    if (timer->isActive()==true){ timer->stop();}
    ui.objId->setText("1.3.6.1.2.1.1.6.0");
    snmpGet("1.3.6.1.2.1.1.6.0");
    ui.label_value->setText(snmpget_value);
}

void manageDialog::snmpWalk(){
    if (timer->isActive()==true){ timer->stop();}
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);

    // Set filter for logging
    DefaultLog::log()->set_filter(ERROR_LOG, 7);
    DefaultLog::log()->set_filter(WARNING_LOG, 7);
    DefaultLog::log()->set_filter(EVENT_LOG, 7);
    DefaultLog::log()->set_filter(INFO_LOG, 7);
    DefaultLog::log()->set_filter(DEBUG_LOG, 7);

    Snmp::socket_startup();

    //---------[ make a GenAddress and Oid object to retrieve ]---------------
    UdpAddress address(qPrintable(getAddress().append('/').append(getPort())));      // make a SNMP++ Generic address
    if ( !address.valid()) {           // check validity of address
           return;
    }
    Oid oid("1.3.6.1.2.1.2.2.1.7");      // default is sysDescr

    //---------[ determine options to use ]-----------------------------------
    snmp_version version=version1;                  // default is v1
    QString mysnmpversion=getVersion();
    if (mysnmpversion.operator ==("version3")){
        version=version3;
        }
        else if (mysnmpversion.operator ==("version2c")){
            version=version2c;
        }
        else{
            version=version1;
        }
    int retries=ui.manage_retries->value();                                  // default retries is 1
    int timeout=ui.manage_timeout->value();                                // default is 1 second
    unsigned short int port=getPort();                               // default snmp port is 161
    OctetStr community(qPrintable(getCommunity()));                   // community name
    bool subtree = true;
    int requests = 0;        // keep track of # of requests
    int objects  = 0;
    #ifdef _SNMPv3
    //OctetStr privPassword("");
    OctetStr privPassword(qPrintable(getprivPassword()));

    //OctetStr authPassword("");
    OctetStr authPassword(qPrintable(getauthPassword()));

    //OctetStr securityName("");
    OctetStr securityName(qPrintable(getsecurityName()));

    int securityModel = SNMP_SECURITY_MODEL_USM;

    //int securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;
    int securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;

    //OctetStr contextName("");
    OctetStr contextName(qPrintable(getContextName()));

    //OctetStr contextEngineID("");
    OctetStr contextEngineID(qPrintable(getContextEngineID()));

    //long authProtocol = SNMP_AUTHPROTOCOL_NONE;
    long authProtocol = SNMP_AUTHPROTOCOL_NONE;

    //long privProtocol = SNMP_PRIVPROTOCOL_NONE;
    long privProtocol = SNMP_PRIVPROTOCOL_NONE;

    v3MP *v3_MP;

    #endif

    #ifdef _SNMPv3


    //Set Security Level
    if (getsecurityLevel().operator ==("noAuthNoPriv"))
        securityLevel = SNMP_SECURITY_LEVEL_NOAUTH_NOPRIV;
    else if (getsecurityLevel().operator ==("authNoPriv"))
       securityLevel = SNMP_SECURITY_LEVEL_AUTH_NOPRIV;
    else
       securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;

    //Set authProtocol
    if (getauthProtocol().operator ==("SHA"))
      authProtocol = SNMP_AUTHPROTOCOL_HMACSHA;
    else if (getauthProtocol().operator ==("MD5"))
      authProtocol = SNMP_AUTHPROTOCOL_HMACMD5;
    else if (getauthProtocol().operator ==("NONE"))
      authProtocol = SNMP_AUTHPROTOCOL_NONE;
    else
      cout << "Warning: ignoring unknown auth protocol: " << qPrintable(getauthProtocol()) << endl;

    //Set privProtocol
    if (getprivProtocol().operator ==("DES"))
        privProtocol = SNMP_PRIVPROTOCOL_DES;
    else if (getprivProtocol().operator ==("3DESEDE"))
        privProtocol = SNMP_PRIVPROTOCOL_3DESEDE;
    else if (getprivProtocol().operator ==("IDEA"))
        privProtocol = SNMP_PRIVPROTOCOL_IDEA;
    else if (getprivProtocol().operator ==("AES128"))
        privProtocol = SNMP_PRIVPROTOCOL_AES128;
    else if (getprivProtocol().operator ==("AES192"))
        privProtocol = SNMP_PRIVPROTOCOL_AES192;
    else if (getprivProtocol().operator ==("AES256"))
        privProtocol = SNMP_PRIVPROTOCOL_AES256;
    else if (getprivProtocol().operator ==("NONE"))
        privProtocol = SNMP_PRIVPROTOCOL_NONE;
    else
    cout << "Warning: ignoring unknown priv protocol: " << qPrintable(getprivProtocol()) << endl;

         //Set Security Model
         //  if (( securityModel < SNMP_SECURITY_MODEL_V1) ||
         //      ( securityModel > SNMP_SECURITY_MODEL_USM))
         //    securityModel = SNMP_SECURITY_MODEL_USM;

    #endif
    //----------[ create a SNMP++ session ]-----------------------------------
    int status;
    // bind to any port and use IPv6 if needed
    Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

    if ( status != SNMP_CLASS_SUCCESS) {
       cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "";
       //ui.status_snmpWalk->setText("SNMP++ Session Create Fail, " +snmp.error_msg(status));
       return;
    }

    //---------[ init SnmpV3 ]--------------------------------------------
    #ifdef _SNMPv3
       if (version == version3) {
         const char *engineId = "snmpWalk";
         const char *filename = "snmpv3_boot_counter";
         unsigned int snmpEngineBoots = 0;
         int status;

         status = getBootCounter(filename, engineId, snmpEngineBoots);
         if ((status != SNMPv3_OK) && (status < SNMPv3_FILEOPEN_ERROR))
         {
           cout << "Error loading snmpEngineBoots counter: " << status << endl;
           ui.status_snmpWalk->setText("Error loading snmpEngineBoots counter: "+status);
           return ;
         }
         snmpEngineBoots++;
         status = saveBootCounter(filename, engineId, snmpEngineBoots);
         if (status != SNMPv3_OK)
         {
           cout << "Error saving snmpEngineBoots counter: " << status << endl;
           ui.status_snmpWalk->setText("Error saving snmpEngineBoots counter: "+status);
           return ;
         }

         int construct_status;
         v3_MP = new v3MP(engineId, snmpEngineBoots, construct_status);
         if (construct_status != SNMPv3_MP_OK)
         {
           cout << "Error initializing v3MP: " << construct_status << endl;
           ui.status_snmpWalk->setText("Error initializing v3MP: "+construct_status);
           return ;
         }

         USM *usm = v3_MP->get_usm();
         usm->add_usm_user(securityName,
                           authProtocol, privProtocol,
                           authPassword, privPassword);
       }
       else
       {
         // MUST create a dummy v3MP object if _SNMPv3 is enabled!
         int construct_status;
         v3_MP = new v3MP("dummy", 0, construct_status);
       }
    #endif

       //--------[ build up SNMP++ object needed ]-------------------------------
       Pdu pdu;                               // construct a Pdu object
       Vb vb;                                 // construct a Vb object
       vb.set_oid("1.3.6.1.2.1.2.2.1.7");     // set the Oid portion of the Vb
       pdu += vb;                             // add the vb to the Pdu

       address.set_port(port);
       CTarget ctarget( address);             // make a target using the address

#ifdef _SNMPv3
       UTarget utarget( address);

       if (version == version3) {
         utarget.set_version( version);          // set the SNMP version SNMPV1 or V2 or V3
         utarget.set_retry( retries);            // set the number of auto retries
         utarget.set_timeout( timeout);          // set timeout
         utarget.set_security_model( securityModel);
         utarget.set_security_name( securityName);
         pdu.set_security_level( securityLevel);
         pdu.set_context_name (contextName);
         pdu.set_context_engine_id(contextEngineID);
       }
       else {
    #endif
         ctarget.set_version( version);         // set the SNMP version SNMPV1 or V2
         ctarget.set_retry( retries);           // set the number of auto retries
         ctarget.set_timeout( timeout);         // set timeout
         ctarget.set_readcommunity( community); // set the read community name
    #ifdef _SNMPv3
       }
    #endif

       //-------[ issue the request, blocked mode ]-----------------------------
       cout << "SNMP++ snmpWalk to " << qPrintable(getAddress()) << " SNMPV"
    #ifdef _SNMPv3
            << ((version==version3) ? (version) : (version+1))
    #else
            << (version+1)
    #endif
            << " Retries=" << retries
            << " Timeout=" << timeout * 10 <<"ms";
    #ifdef _SNMPv3
       if (version == version3)
         cout << endl
              << "securityName= " << securityName.get_printable()
              << ", securityLevel= " << securityLevel
              << ", securityModel= " << securityModel << endl
              << "contextName= " << contextName.get_printable()
              << ", contextEngineID= " << contextEngineID.get_printable()
              << endl;
       else
    #endif
         cout << " Community=" << community.get_printable() << endl << flush;

       SnmpTarget *target;
    #ifdef _SNMPv3
       if (version == version3)
         target = &utarget;
       else
    #endif
         target = &ctarget;
                int row=0;
    int column=0;
     SmiUINT32 val;
       while (( status = snmp.get_bulk( pdu,*target,0,BULK_MAX))== SNMP_CLASS_SUCCESS) {
       column=0;
              requests++;
              for ( int z=0;z<pdu.get_vb_count(); z++) {
                 pdu.get_vb( vb,z);
    #ifdef _SNMPv3
                 if (pdu.get_type() == REPORT_MSG) {
                   Oid tmp;
                   vb.get_oid(tmp);
                   cout << "Received a reportPdu: "
                        << snmp.error_msg( tmp)
                        << endl
                        << vb.get_printable_oid() << " = "
                        << vb.get_printable_value() << endl;
                   //ui.status_snmpWalk->setText("Received a reportPdu: "+snmp.error_msg( tmp)+ vb.get_printable_oid()+" = "+vb.get_printable_value());
                   return ;
                 }
    #endif
                 Oid tmp;
                 vb.get_oid(tmp);
                 if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
                 {
                     cout << "End of SUBTREE Reached\n";
                     cout << "Total # of Requests = " << requests << "\n";         
                     cout << "Total # of Objects  = " << objects  << "\n";
                     //ui.status_snmpWalk->setText("Total # of Requests = "+requests+ " Total # of Objects  = "+objects );
                     return ;
                 }
                     objects++;
                     // look for var bind exception, applies to v2 only
                     if ( vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW) {
                       cout << vb.get_printable_oid() << " = ";
                       cout << vb.get_printable_value() << "\n";
            ui.tableWidget->insertRow(row);

            QTableWidgetItem *new_obj_id = new QTableWidgetItem(tr("%1").arg(vb.get_printable_oid()));
            new_obj_id->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui.tableWidget->setItem(row, column, new_obj_id);
            column++;
            QTableWidgetItem *new_obj_val = new QTableWidgetItem(tr("%1").arg(vb.get_printable_value()));
            new_obj_val->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui.tableWidget->setItem(row, column, new_obj_val);
            column++;
            val=vb.get_syntax();         
            QTableWidgetItem *new_obj_synt = new QTableWidgetItem(tr("%1").arg(vb_to_str(val)));
            new_obj_synt->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui.tableWidget->setItem(row, column, new_obj_synt);
            row++;
                     }
                     else {
                       cout << "End of MIB Reached\n";
                       cout << "Total # of Requests = " << requests << "\n";
                       cout << "Total # of Objects  = " << objects  << "\n";
                       //ui.status_snmpWalk->setText("Total # of Requests = "+requests+ " Total # of Objects  = "+objects );
                       return ;
                     }
              }
              // last vb becomes seed of next rquest
              pdu.set_vblist(&vb, 1);
       }
       if ( status != SNMP_ERROR_NO_SUCH_NAME)
         cout << "SNMP++ snmpWalk Error, " << snmp.error_msg( status) << "\n";
       cout << "Total # of Requests = " << requests << "\n";
       cout << "Total # of Objects  = " << objects  << "\n";
       //ui.status_snmpWalk->setText("SNMP++ snmpWalk Error:Total # of Requests = "+requests+ " Total # of Objects  = "+objects );
       Snmp::socket_cleanup();  // Shut down socket subsystem
    }

void manageDialog::on_change_cell_on_table(int row, int column){
    if (timer->isActive()==true){ timer->stop();}
    QTableWidgetItem *item_clicked_1=new QTableWidgetItem();
    QTableWidgetItem *item_clicked_2=new QTableWidgetItem();
    QTableWidgetItem *item_clicked_3=new QTableWidgetItem();
    if (column==0){
        item_clicked_1=ui.tableWidget->item(row,column);
        item_clicked_2=ui.tableWidget->item(row,column+1);
        item_clicked_3=ui.tableWidget->item(row,column+2);
        ui.objId->setText(item_clicked_1->text());
        if_num=item_clicked_1->text().section('.',-1);
        timer->start(ui.poll_ms->value());
    }
    else if (column==1){
        item_clicked_1=ui.tableWidget->item(row,column);
        item_clicked_2=ui.tableWidget->item(row,column-1);
        item_clicked_3=ui.tableWidget->item(row,column+1);
        ui.objId->setText(item_clicked_2->text());
        if_num=item_clicked_2->text().section('.',-1);
        timer->start(ui.poll_ms->value());
    }
    else if (column==2) {
        item_clicked_1=ui.tableWidget->item(row,column);
        item_clicked_2=ui.tableWidget->item(row,column-2);
        item_clicked_3=ui.tableWidget->item(row,column-1);
        ui.objId->setText(item_clicked_2->text());
        if_num=item_clicked_2->text().section('.',-1);
        timer->start(ui.poll_ms->value());
    }
}

void manageDialog::snmpGet(QString oid_str){
    // Set filter for logging
    DefaultLog::log()->set_filter(ERROR_LOG, 7);
    DefaultLog::log()->set_filter(WARNING_LOG, 7);
    DefaultLog::log()->set_filter(EVENT_LOG, 7);
    DefaultLog::log()->set_filter(INFO_LOG, 7);
    DefaultLog::log()->set_filter(DEBUG_LOG, 7);

    Snmp::socket_startup();

    //---------[ make a GenAddress and Oid object to retrieve ]---------------
    UdpAddress address(qPrintable(getAddress().append('/').append(getPort())));      // make a SNMP++ Generic address
    if ( !address.valid()) {           // check validity of address
           return ;
    }

    Oid oid(qPrintable(oid_str));      // default is sysDescr

    //---------[ determine options to use ]-----------------------------------
    snmp_version version=version1;                  // default is v1
    QString mysnmpversion=getVersion();
    if (mysnmpversion.operator ==("version3")){
        version=version3;
        }
        else if (mysnmpversion.operator ==("version2c")){
            version=version2c;
        }
        else{
            version=version1;
        }
    int retries=ui.manage_retries->value();                                  // default retries is 1
    int timeout=ui.manage_timeout->value();                                // default is 1 second
    unsigned short int port=getPort();                               // default snmp port is 161
    OctetStr community(qPrintable(getCommunity()));                   // community name

    #ifdef _SNMPv3
    //OctetStr privPassword("");
    OctetStr privPassword(qPrintable(getprivPassword()));

    //OctetStr authPassword("");
    OctetStr authPassword(qPrintable(getauthPassword()));

    //OctetStr securityName("");
    OctetStr securityName(qPrintable(getsecurityName()));

    int securityModel = SNMP_SECURITY_MODEL_USM;

    //int securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;
    int securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;

    //OctetStr contextName("");
    OctetStr contextName(qPrintable(getContextName()));

    //OctetStr contextEngineID("");
    OctetStr contextEngineID(qPrintable(getContextEngineID()));

    //long authProtocol = SNMP_AUTHPROTOCOL_NONE;
    long authProtocol = SNMP_AUTHPROTOCOL_NONE;

    //long privProtocol = SNMP_PRIVPROTOCOL_NONE;
    long privProtocol = SNMP_PRIVPROTOCOL_NONE;

    v3MP *v3_MP;

    //Set Security Level
    if (getsecurityLevel().operator ==("noAuthNoPriv"))
        securityLevel = SNMP_SECURITY_LEVEL_NOAUTH_NOPRIV;
    else if (getsecurityLevel().operator ==("authNoPriv"))
       securityLevel = SNMP_SECURITY_LEVEL_AUTH_NOPRIV;
    else
       securityLevel = SNMP_SECURITY_LEVEL_AUTH_PRIV;

    //Set authProtocol
    if (getauthProtocol().operator ==("SHA"))
      authProtocol = SNMP_AUTHPROTOCOL_HMACSHA;
    else if (getauthProtocol().operator ==("MD5"))
      authProtocol = SNMP_AUTHPROTOCOL_HMACMD5;
    else if (getauthProtocol().operator ==("NONE"))
      authProtocol = SNMP_AUTHPROTOCOL_NONE;
    else
      cout << "Warning: ignoring unknown auth protocol: " << qPrintable(getauthProtocol()) << endl;

    //Set privProtocol
    if (getprivProtocol().operator ==("DES"))
        privProtocol = SNMP_PRIVPROTOCOL_DES;
    else if (getprivProtocol().operator ==("3DESEDE"))
        privProtocol = SNMP_PRIVPROTOCOL_3DESEDE;
    else if (getprivProtocol().operator ==("IDEA"))
        privProtocol = SNMP_PRIVPROTOCOL_IDEA;
    else if (getprivProtocol().operator ==("AES128"))
        privProtocol = SNMP_PRIVPROTOCOL_AES128;
    else if (getprivProtocol().operator ==("AES192"))
        privProtocol = SNMP_PRIVPROTOCOL_AES192;
    else if (getprivProtocol().operator ==("AES256"))
        privProtocol = SNMP_PRIVPROTOCOL_AES256;
    else if (getprivProtocol().operator ==("NONE"))
        privProtocol = SNMP_PRIVPROTOCOL_NONE;
    else
    cout << "Warning: ignoring unknown priv protocol: " << qPrintable(getprivProtocol()) << endl;

         /*//Set Security Model
           if (( securityModel < SNMP_SECURITY_MODEL_V1) ||
               ( securityModel > SNMP_SECURITY_MODEL_USM))
             securityModel = SNMP_SECURITY_MODEL_USM;
          */
    #endif
    //----------[ create a SNMP++ session ]-----------------------------------
    int status;
    // bind to any port and use IPv6 if needed
    Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

    if ( status != SNMP_CLASS_SUCCESS) {
       cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "";
       return;
    }
    //---------[ init SnmpV3 ]--------------------------------------------
 #ifdef _SNMPv3
    if (version == version3) {
      const char *engineId = "snmpGet";
      const char *filename = "snmpv3_boot_counter";
      unsigned int snmpEngineBoots = 0;
      int status;

      status = getBootCounter(filename, engineId, snmpEngineBoots);
      if ((status != SNMPv3_OK) && (status < SNMPv3_FILEOPEN_ERROR))
      {
        cout << "Error loading snmpEngineBoots counter: " << status << endl;
        return;
      }
      snmpEngineBoots++;
      status = saveBootCounter(filename, engineId, snmpEngineBoots);
      if (status != SNMPv3_OK)
      {
        cout << "Error saving snmpEngineBoots counter: " << status << endl;
        return;
      }

      int construct_status;
      v3_MP = new v3MP(engineId, snmpEngineBoots, construct_status);
      if (construct_status != SNMPv3_MP_OK)
      {
        cout << "Error initializing v3MP: " << construct_status << endl;
        return;
      }

      USM *usm = v3_MP->get_usm();
      usm->add_usm_user(securityName,
                        authProtocol, privProtocol,
                        authPassword, privPassword);
    }
    else
    {
      // MUST create a dummy v3MP object if _SNMPv3 is enabled!
      int construct_status;
      v3_MP = new v3MP("dummy", 0, construct_status);
    }
 #endif

    //--------[ build up SNMP++ object needed ]-------------------------------
    Pdu pdu;                               // construct a Pdu object
    Vb vb;                                 // construct a Vb object
    vb.set_oid( oid);                      // set the Oid portion of the Vb
    pdu += vb;                             // add the vb to the Pdu

    address.set_port(port);
    CTarget ctarget( address);             // make a target using the address
 #ifdef _SNMPv3
    UTarget utarget( address);

    if (version == version3) {
      utarget.set_version( version);          // set the SNMP version SNMPV1 or V2 or V3
      utarget.set_retry( retries);            // set the number of auto retries
      utarget.set_timeout( timeout);          // set timeout
      utarget.set_security_model( securityModel);
      utarget.set_security_name( securityName);
      pdu.set_security_level( securityLevel);
      pdu.set_context_name (contextName);
      pdu.set_context_engine_id(contextEngineID);
    }
    else {
 #endif
      ctarget.set_version( version);         // set the SNMP version SNMPV1 or V2
      ctarget.set_retry( retries);           // set the number of auto retries
      ctarget.set_timeout( timeout);         // set timeout
      ctarget.set_readcommunity( community); // set the read community name
 #ifdef _SNMPv3
    }
 #endif

    //-------[ issue the request, blocked mode ]-----------------------------
    cout << "SNMP++ Get to " << qPrintable(getAddress()) << " SNMPV"
 #ifdef _SNMPv3
         << ((version==version3) ? (version) : (version+1))
 #else
         << (version+1)
 #endif
         << " Retries=" << retries
         << " Timeout=" << timeout * 10 <<"ms";
 #ifdef _SNMPv3
    if (version == version3)
      cout << endl
           << "securityName= " << securityName.get_printable()
           << ", securityLevel= " << securityLevel
           << ", securityModel= " << securityModel << endl
           << "contextName= " << contextName.get_printable()
           << ", contextEngineID= " << contextEngineID.get_printable()
           << endl;
    else
 #endif
      cout << " Community=" << community.get_printable() << endl << flush;

    SnmpTarget *target;
 #ifdef _SNMPv3
    if (version == version3)
      target = &utarget;
    else
 #endif
      target = &ctarget;

    status = snmp.get( pdu, *target);

    if (status == SNMP_CLASS_SUCCESS)
    {
      pdu.get_vb( vb,0);
 #ifdef _SNMPv3
      if (pdu.get_type() == REPORT_MSG) {
        cout << "Received a report pdu: "
             << snmp.error_msg(vb.get_printable_oid()) << endl;
      }
 #endif
      cout << "Oid = " << vb.get_printable_oid() << endl
           << "Value = " << vb.get_printable_value() << endl;
      snmpget_value=vb.get_printable_value();
      SmiUINT32 val=vb.get_syntax();
      ui.label_value->setText(vb.get_printable_value());
      ui.label_syntax->setText(vb_to_str(val));
      if ((vb.get_syntax() == sNMP_SYNTAX_ENDOFMIBVIEW) ||
          (vb.get_syntax() == sNMP_SYNTAX_NOSUCHINSTANCE) ||
          (vb.get_syntax() == sNMP_SYNTAX_NOSUCHOBJECT))
        cout << "Exception: " << vb.get_syntax() << " occured." << endl;
    }
    else
    {
      cout << "SNMP++ Get Error, " << snmp.error_msg( status)
           << " (" << status <<")" << endl;
    }

    Snmp::socket_cleanup();  // Shut down socket subsystem
    return;
 }

QString manageDialog::vb_to_str(SmiUINT32 val)
{
    QString syntax;
    if (val==4){
        syntax="Octet String";
    }
    else if (val==64)
    {
        syntax="IP Address";
    }
    else if (val==6)
    {
        syntax="Oid";
    }
    else if (val==67)
    {
        syntax="TimeTicks";
    }
    else if (val==66)
    {
        syntax="Gauge32";
    }
    else if (val==65)
    {
        syntax="Counter32";
    }
    else if (val==2)
    {
        syntax="Integer";
    }
    else if (val==sNMP_SYNTAX_CNTR64)
    {
        syntax="Counter64";
    }
    else if (val==sNMP_SYNTAX_NOSUCHOBJECT)
    {
        syntax="NOSUCHOBJECT";
    }
    else{
        syntax="ERROR!";
    }
    return syntax;
}

void manageDialog::statistics()
{
    prev_ifInOctets=0;
    prev_ifOutOctets=0;
    prev_timeticks=0;
    prev_ifInUcastPkts=0;
    prev_ifInNUcastPkts=0;
    prev_ifOutUcastPkts=0;
    prev_ifOutNUcastPkts=0;

    QString oid_str, uptime;
    bool ok;
    long ifInOctets, ifOutOctets, throughput_bits_out, throughput_bits_in, throughput_bits_total, throughput_packets_in, throughput_packets_out, throughput_packets_total, minutes, hours, timeticks, ifSpeed, utilization;
    long ifInUcastPkts, ifInNUcastPkts, ifInDiscards, ifInErrors, ifInUnknownProtos, ifOutUcastPkts, ifOutNUcastPkts, ifOutDiscards, ifOutErrors;
    double p_in, p_out, per_in, per_out, per_rate_in, per_rate_out, hundrendsofSeconds;

    //snmpget sysUpTime
    snmpGet("1.3.6.1.2.1.1.3.0");
    uptime=snmpget_value;
    //snmpget ifSpeed
    oid_str="1.3.6.1.2.1.2.2.1.5.";
    snmpGet(oid_str.append(if_num));
    ifSpeed=snmpget_value.toLong(&ok, 10);
    //snmpget ifInOctets
    oid_str="1.3.6.1.2.1.2.2.1.10.";
    snmpGet(oid_str.append(if_num));
    ifInOctets=snmpget_value.toLong(&ok, 10);
    //snmpget ifInUcastPkts
    oid_str="1.3.6.1.2.1.2.2.1.11.";
    snmpGet(oid_str.append(if_num));
    ifInUcastPkts=snmpget_value.toLong(&ok, 10);
    //snmpget ifInNUcastPkts
    oid_str="1.3.6.1.2.1.2.2.1.12.";
    snmpGet(oid_str.append(if_num));
    ifInNUcastPkts=snmpget_value.toLong(&ok, 10);
    //snmpget ifInDiscards
    oid_str="1.3.6.1.2.1.2.2.1.13.";
    snmpGet(oid_str.append(if_num));
    ifInDiscards=snmpget_value.toLong(&ok, 10);
    //snmpget ifInErrors
    oid_str="1.3.6.1.2.1.2.2.1.14.";
    snmpGet(oid_str.append(if_num));
    ifInErrors=snmpget_value.toLong(&ok, 10);
    //snmpget ifInUnknownProtos
    oid_str="1.3.6.1.2.1.2.2.1.15.";
    snmpGet(oid_str.append(if_num));
    ifInUnknownProtos=snmpget_value.toLong(&ok, 10);
    //snmpget ifOutOctets
    oid_str="1.3.6.1.2.1.2.2.1.16.";
    snmpGet(oid_str.append(if_num));
    ifOutOctets=snmpget_value.toLong(&ok, 10);
    //snmpget ifOutUcastPkts
    oid_str="1.3.6.1.2.1.2.2.1.17.";
    snmpGet(oid_str.append(if_num));
    ifOutUcastPkts=snmpget_value.toLong(&ok, 10);
    //snmpget ifOutNUcastPkts
    oid_str="1.3.6.1.2.1.2.2.1.18.";
    snmpGet(oid_str.append(if_num));
    ifOutNUcastPkts=snmpget_value.toLong(&ok, 10);
    //snmpget ifOutDiscards
    oid_str="1.3.6.1.2.1.2.2.1.19.";
    snmpGet(oid_str.append(if_num));
    ifOutDiscards=snmpget_value.toLong(&ok, 10);
    //snmpget ifOutErrors
    oid_str="1.3.6.1.2.1.2.2.1.20.";
    snmpGet(oid_str.append(if_num));
    ifOutErrors=snmpget_value.toLong(&ok, 10);

    hours=uptime.section(':',0,0).toLong(&ok, 10)*60*60*100;
    minutes=uptime.section(':',1,1).toLong(&ok, 10)*60*100;
    hundrendsofSeconds=uptime.section(':',2.2).toDouble()*100;
    timeticks=hours + minutes + hundrendsofSeconds;

    //Compute P
    if (ifInUcastPkts==0)
    {
        ui.p_in->setText("inf");
    }
    else
    {
        p_in=(ifInNUcastPkts/ifInUcastPkts)*100;
        ui.p_in->setText(QString::number(p_in,'g',10));
    }
    if (ifOutUcastPkts==0)
    {
        ui.p_out->setText("inf");
    }
    else
    {
        p_out=(ifOutNUcastPkts/ifOutUcastPkts)*100;
        ui.p_out->setText(QString::number(p_out,'g',10));
    }

    //Compute Packet Error Probability
    if ((ifInUcastPkts+ifInNUcastPkts)==0)
    {
        ui.per_in->setText("inf");
    }
    else
    {
        per_in=(ifInErrors+ifInDiscards+ifInUnknownProtos)/(ifInUcastPkts+ifInNUcastPkts);
        per_rate_in=per_in/((timeticks-prev_timeticks)/100);
        ui.per_in->setText(QString::number(per_in,'g',10));
        ui.per_rate_in->setText(QString::number(per_rate_in,'g',10));
    }
    if ((ifOutUcastPkts+ifOutNUcastPkts)==0)
    {
        ui.per_out->setText("inf");
    }
    else
    {
        per_out=(ifOutErrors+ifOutDiscards)/(ifOutUcastPkts+ifOutNUcastPkts);
        per_rate_out=per_out/((timeticks-prev_timeticks)/100);
        ui.per_out->setText(QString::number(per_out,'g',10));
        ui.per_rate_out->setText(QString::number(per_rate_out,'g',10));
    }
    //Compute throughput (bits/sec)
    throughput_bits_out=((ifOutOctets-prev_ifOutOctets)*800)/((timeticks-prev_timeticks)/100);
    throughput_bits_in=((ifInOctets-prev_ifOutOctets)*800)/((timeticks-prev_timeticks)/100);
    throughput_bits_total = throughput_bits_out + throughput_bits_in;

    //Compute throughput (packets/sec)
    throughput_packets_in=(ifInUcastPkts-prev_ifInUcastPkts)/((timeticks-prev_timeticks)/100);
    throughput_packets_out=(ifOutUcastPkts-prev_ifOutUcastPkts)/((timeticks-prev_timeticks)/100);
    throughput_packets_total = throughput_packets_out + throughput_packets_in;

    //Compute utilization
    if (ifSpeed==0)
    {
        ui.utilization->setText("ifSpeed is 0");
    }
    else
    {
        ui.ifSpeed->setText(QString::number(ifSpeed));
        utilization=throughput_bits_total/ifSpeed;
        ui.utilization->setText(QString::number(utilization));
    }
    //Print values to the GUI
    ui.uptime->setText(uptime);
    ui.timeticks->setText(QString::number(timeticks,10));

    ui.throughput_bits_out->setText(QString::number(throughput_bits_out));
    ui.throughput_bits_in->setText(QString::number(throughput_bits_in));
    ui.throughput_bits_total->setText(QString::number(throughput_bits_total));

    ui.throughput_packets_out->setText(QString::number(throughput_packets_out));
    ui.throughput_packets_in->setText(QString::number(throughput_packets_in));
    ui.throughput_packets_total->setText(QString::number(throughput_packets_total));


    prev_ifInOctets=ifInOctets;
    prev_ifOutOctets=ifOutOctets;
    prev_timeticks=timeticks;
    prev_ifInUcastPkts=ifInUcastPkts;
    prev_ifInNUcastPkts=ifInNUcastPkts;
    prev_ifOutUcastPkts=ifOutUcastPkts;
    prev_ifOutNUcastPkts=ifOutNUcastPkts;
    QString message="Polling interface ";
    message.append(if_num);
    ui.status_snmpWalk->setText(message);
}

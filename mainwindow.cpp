#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutDialog.h"
#include "manageDialog.h"
#include <QtGui>
#include <QList>
#include <QTreeWidgetItem>
#include "snmp++/include/snmp_pp/snmp_pp.h"
#include "snmp++/include/snmp_pp/collect.h"
#include "snmp++/include/snmp_pp/address.h"
#include "snmp++/include/snmp_pp/oid.h"

//TODO Headers to create dynamically updated plots
//#include "qwt_plot.h"
//#include "qwt_plot_curve.h"


#include <iostream>
#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif
using namespace std;

#define SYSDESCR "1.3.6.1.2.1.1.1.0"              // Object ID for System Descriptor

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("SNMP MANAGER"));
    connect(ui->button_broadcast, SIGNAL(clicked()), this, SLOT (broadcast()));
    connect(ui->actionAbout, SIGNAL(clicked()), this, SLOT (on_actionAbout_triggered()));
    connect(ui->tree_nodes, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onDoubleClickAddDesignItem(QTreeWidgetItem*, int)));
    connect(ui->pushButton_manualManage, SIGNAL(clicked()), this, SLOT (on_click_pushButton_manualManage()));
    connect(ui->radio_button_v1, SIGNAL(clicked()), this, SLOT(button_group_version_clicked()));
    connect(ui->radio_button_v2, SIGNAL(clicked()), this, SLOT(button_group_version_clicked()));
    connect(ui->radio_button_v3, SIGNAL(clicked()), this, SLOT(button_group_version_clicked()));
    ui->menuView->addAction(ui->dockWidget->toggleViewAction());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::button_group_version_clicked()
{
    if (ui->radio_button_v3->isChecked()){
        ui->groupBox_v1v2->setEnabled(false);
        ui->groupBox_v3->setEnabled(true);
    }
    else if (ui->radio_button_v2->isChecked()){
        ui->groupBox_v1v2->setEnabled(true);
        ui->groupBox_v3->setEnabled(false);
    }
    else{
        ui->groupBox_v1v2->setEnabled(true);
        ui->groupBox_v3->setEnabled(false);
  }
}
// Display About Dialog
bool MainWindow::on_actionAbout_triggered()
{
    aboutDialog *about = new aboutDialog(this);
    if (!about->exec())
             return false;
    return true;
}


bool MainWindow::get_system_descriptor(QString addr_str)
{
   int status;                                      // return status

   CTarget ctarget( (IpAddress) qPrintable(addr_str));        // SNMP++ community target
   //CTarget ctarget( (IpAddress) "127.0.0.1");        // SNMP++ community target

   Vb vb( SYSDESCR);                             // SNMP++ Variable Binding Object

   Pdu pdu;                                                 // SNMP++ PDU


    //-------[ Construct a SNMP++ SNMP Object ]---------------------------------------
    #ifdef Q_OS_WIN32
        Snmp::socket_startup();
    #endif
   Snmp snmp( status);                             // Create a SNMP++ session

   if ( status != SNMP_CLASS_SUCCESS) {          // check creation status

      cout<< snmp.error_msg( status);                   //  if fail, print error string
      ui->label_manualStatus->setText(snmp.error_msg( status));
      return false;  }



   //-------[ Invoke a SNMP++ Get ]-------------------------------------------------------

   pdu += vb;                                    //  add the variable binding to the PDU

   if (  (status = snmp.get(  pdu, ctarget)) != SNMP_CLASS_SUCCESS)
   {

      cout<< snmp.error_msg( status);
      ui->label_manualStatus->setText(snmp.error_msg( status));
      return false;
   }
   else {

      pdu.get_vb( vb,0);                               // extract the variable binding from PDU

      cout << "System Descriptor = "<< vb.get_printable_value();   // print out the value
      return true;
  }


};  // Thats all!

void MainWindow::broadcast(){
   // Set filter for logging
   DefaultLog::log()->set_filter(ERROR_LOG, 7);
   DefaultLog::log()->set_filter(WARNING_LOG, 7);
   DefaultLog::log()->set_filter(EVENT_LOG, 7);
   DefaultLog::log()->set_filter(INFO_LOG, 7);
   DefaultLog::log()->set_filter(DEBUG_LOG, 7);

   Snmp::socket_startup();  // Initialize socket subsystem

   //---------[ make a GenAddress and Oid object to retrieve ]---------------
   // Create a address object from the entered value
   broadcast_addr=ui->line_edit_target->text();
   broadcast_port=ui->line_edit_port->text();
   addr_with_port=ui->line_edit_target->text().append('/').append(ui->line_edit_port->text());
   UdpAddress address(qPrintable(addr_with_port));

   // check if the address is valid
   // One problem here: if a hostname is entered, a blocking DNS lookup
   // is done by the address object.
   if (!address.valid()){
     QString err = QString("Invalid Address or DNS Name: %1")
                 .arg(qPrintable(addr_with_port));
     ui->status_nodes->setText(err);
     return;
   }
   Oid oid("1.3.6.1.2.1.1.1.0");      // default is sysDescr

   //---------[ determine options to use ]-----------------------------------
   snmp_version version=version1;       // default is v1
   if (ui->radio_button_v3->isChecked())
     version = version3;
   else if (ui->radio_button_v2->isChecked())
     version = version2c;
   else
     version = version1;

   // Get retries, timeout and community
   bool ok;
   int retries =ui->spin_box_retries->value();
   int timeout = 100 * ui->spin_box_timeout->value();
   u_short port= ui->line_edit_port->text().toUShort(&ok,10);           // default snmp port is 161
   OctetStr community (qPrintable(ui->line_edit_community->text()));    // community name

#ifdef _SNMPv3
   v3MP *v3_MP;
#endif


   //----------[ create a SNMP++ session ]-----------------------------------
   int status;
   // bind to any port and use IPv6 if needed
   Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

   if ( status != SNMP_CLASS_SUCCESS) {
      cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "";
      ui->status_nodes->setText(snmp.error_msg(status));
      return;
   }
/*
   //---------[ init SnmpV3 ]--------------------------------------------
#ifdef _SNMPv3
   // MUST create a dummy v3MP object if _SNMPv3 is enabled!
   int construct_status;
   v3_MP = new v3MP("dummy", 0, construct_status);
   if (construct_status != SNMPv3_MP_OK)
   {
     cout << "Error initializing v3MP: " << construct_status << endl;
     return;
   }
#endif
*/
   //--------[ build up SNMP++ object needed ]-------------------------------
   address.set_port(port);

   //-------[ issue the broadcast, blocked mode ]----------------------------
   cout << "SNMP++ broadcast to " << qPrintable(addr_with_port) << " SNMPV"
#ifdef _SNMPv3
        << ((version==version3) ? (version) : (version+1))
#else
        << (version+1)
#endif
        << " Retries=" << retries
        << " Timeout=" << timeout * 10 <<"ms";
#ifdef _SNMPv3
   if (version == version3)
     cout << endl;
   else
#endif
     cout << " Community=" << community.get_printable() << endl << flush;
   QList<QTreeWidgetItem *> items;

   UdpAddressCollection addresses;

   // do multiple loops as requested from "retries"
   for (int loops=1; loops<= retries + 1; ++loops)
   {
     status = snmp.broadcast_discovery(addresses,
                                       (timeout + 99) / 100,
                                       address, version, &community);

     if (status == SNMP_CLASS_SUCCESS){
       QString err = QString("Success sending broadcast %1.").arg(loops);
       ui->status_nodes->setText(err);
     }
     else{
       QString err = QString("Could not send broadcast: %1")
         .arg(Snmp::error_msg(status));
       ui->status_nodes->setText(err);
     }
   }

   // filter out duplicates
   UdpAddressCollection filtered_addrs;
   int dummy_pos;

   for (int n=0; n < addresses.size(); ++n)
     if (filtered_addrs.find(addresses[n], dummy_pos) == FALSE)
       filtered_addrs += addresses[n];

   // print out all addressess
   ui->status_nodes->setText(QString("Found %1 agents").arg(filtered_addrs.size()));
   ui->tree_nodes->clear();
   for (int m=0; m < filtered_addrs.size(); ++m){
     items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("%1").arg(filtered_addrs[m].get_printable()))));
     ui->tree_nodes->insertTopLevelItems(0, items);
    }
   Snmp::socket_cleanup();  // Shut down socket subsystem
}

bool MainWindow::onDoubleClickAddDesignItem(QTreeWidgetItem* item, int column){
    bool ok;
    QString target_address=item->text(column).section('/',0,0);
    QString target_port_str=item->text(column).section('/',1,1);
    unsigned short int target_port=target_port_str.toUShort(&ok, 10);
    QString version;
    if (ui->radio_button_v3->isChecked())
      version = "version3";
    else if (ui->radio_button_v2->isChecked())
      version = "version2c";
    else
      version = "version1";

    manageDialog *manage = new manageDialog(this);
    manage->setWindowTitle(tr("Manage").append(" ").append(target_address));
    ui->mdiArea->addSubWindow(manage); 
    manage->setAddress(target_address);
    manage->setPort(target_port);
    manage->setVersion(version);
    manage->setCommunity(ui->line_edit_community->text());

    //User-based Security Model
    manage->setsecurityName(ui->lineEdit_securityName->text());
    manage->setsecurityLevel(ui->comboBox_securityLevel->currentText());

    manage->setauthProtocol(ui->comboBox_authProtocol->currentText());
    manage->setauthPassword(ui->lineEdit_authPassword->text());

    manage->setprivProtocol(ui->comboBox_privProtocol->currentText());
    manage->setprivPassword(ui->lineEdit_privPassword->text());


    manage->setContextName(ui->lineEdit_contextName->text());
    manage->setContextEngineID(ui->lineEdit_contextEngineID->text());


    /*QwtPlot *myPlot = new QwtPlot("Two Curves", parent);

    QwtPlotCurve *curve1 = new QwtPlotCurve("Curve 1");
    QwtPlotCurve *curve2 = new QwtPlotCurve("Curve 2");
    curve1->setData();
    curve2->setData();
    curve1->attach(myPlot);
    curve2->attach(myPlot);

    // finally, refresh the plot
   myPlot->replot();*/
    if (!manage->exec())
        return false;
    return true;
}

bool MainWindow::on_click_pushButton_manualManage(){
    bool ok;
    QString version;
    if (ui->radio_button_v3->isChecked())
      version = "version3";
    else if (ui->radio_button_v2->isChecked())
      version = "version2c";
    else
      version = "version1";

    UdpAddress address(qPrintable(ui->lineEdit_manualAddress->text().append('/').append(qPrintable(ui->lineEdit_manualPort->text()))));      // make a SNMP++ Generic address
    if ( !address.valid()) {           // check validity of address
        ui->label_manualStatus->setText("Target is not valid!");
        return true;
    }
    if(!(get_system_descriptor(ui->lineEdit_manualAddress->text())))
    {
        ui->label_manualStatus->setText("Target is not valid!");
        return true;
    }
    ui->label_manualStatus->setText("Ready");
    manageDialog *manage = new manageDialog(this);
    manage->setWindowTitle(tr("Manage").append(" ").append(ui->lineEdit_manualAddress->text()));
    ui->mdiArea->addSubWindow(manage);
    manage->setAddress(ui->lineEdit_manualAddress->text());
    manage->setPort(ui->lineEdit_manualPort->text().toUShort(&ok,10));
    manage->setVersion(version);
    manage->setCommunity(ui->line_edit_community->text());

    //User-based Security Model
    manage->setsecurityName(ui->lineEdit_securityName->text());
    manage->setsecurityLevel(ui->comboBox_securityLevel->currentText());

    manage->setauthProtocol(ui->comboBox_authProtocol->currentText());
    manage->setauthPassword(ui->lineEdit_authPassword->text());

    manage->setprivProtocol(ui->comboBox_privProtocol->currentText());
    manage->setprivPassword(ui->lineEdit_privPassword->text());


    manage->setContextName(ui->lineEdit_contextName->text());
    manage->setContextEngineID(ui->lineEdit_contextEngineID->text());

    if (!manage->exec())
        return false;
    return true;

}

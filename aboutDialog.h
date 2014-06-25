#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "ui_aboutDialog.h"


class aboutDialog : public QDialog
{
    Q_OBJECT

public:
    aboutDialog(QWidget *parent = 0);

private:
    Ui_about ui;
};

#endif // ABOUTDIALOG_H

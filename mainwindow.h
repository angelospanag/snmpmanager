#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);


private:
    Ui::MainWindow *ui;
    QString broadcast_addr, broadcast_port, addr_with_port;

private slots:
    bool get_system_descriptor(QString);
    void broadcast();
    bool on_actionAbout_triggered();
    bool onDoubleClickAddDesignItem(QTreeWidgetItem*, int);
    void button_group_version_clicked();
    bool on_click_pushButton_manualManage();
};

#endif // MAINWINDOW_H

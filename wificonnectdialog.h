#ifndef WIFICONNECTDIALOG_H
#define WIFICONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class WifiConnectDialog;
}

class NetworkService;

class WifiConnectDialog : public QDialog
{
    Q_OBJECT

public:
    WifiConnectDialog(NetworkService *service, QWidget *parent = 0);
    ~WifiConnectDialog();

private:
    Ui::WifiConnectDialog *ui;
    NetworkService *mCurrentNetworkService = nullptr;

private slots:
    void checkChanged();
    void connectClicked();
    void connectRequestFailed(const QString &error);
    void connectedChanged();

};

#endif // WIFICONNECTDIALOG_H

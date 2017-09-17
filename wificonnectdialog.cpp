#include "wificonnectdialog.h"
#include "ui_wificonnectdialog.h"
#include "networkmanager.h"
#include <QMessageBox>
#include <QDebug>

WifiConnectDialog::WifiConnectDialog(NetworkService *service, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiConnectDialog)
{
    ui->setupUi(this);
    setModal(true);
    mCurrentNetworkService = service;
    ui->labelName->setText(service->name());
    ui->progressBar->setValue(service->strength());
    ui->linePassword->setEnabled(service->securityType() > 1);
    connect(ui->checkBox, SIGNAL(toggled(bool)), SLOT(checkChanged()));
    connect(ui->pushConnect, SIGNAL(clicked(bool)), SLOT(connectClicked()));
    connect(service, SIGNAL(connectRequestFailed(QString)), SLOT(connectRequestFailed(QString)));
    connect(service, SIGNAL(connectedChanged(bool)), SLOT(connectedChanged()));
}

WifiConnectDialog::~WifiConnectDialog()
{
    delete ui;
}

void WifiConnectDialog::checkChanged()
{
    ui->linePassword->setEchoMode(ui->checkBox->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
}

void WifiConnectDialog::connectClicked()
{
    //qDebug() << mCurrentNetworkService-> << " : " << mCurrentNetworkService->available();
    //NetworkManager::instance()->registerAgent(mCurrentNetworkService->path());
    mCurrentNetworkService->setPassphrase(ui->linePassword->text());
    mCurrentNetworkService->requestConnect();
    ui->pushConnect->setEnabled(false);
}

void WifiConnectDialog::connectRequestFailed(const QString &error)
{
    QMessageBox::critical(this, tr("Error"), error);
    ui->pushConnect->setEnabled(true);
}

void WifiConnectDialog::connectedChanged()
{
    if(mCurrentNetworkService->connected())
        close();
}

#include "maindialog.h"
#include "ui_maindialog.h"
#include "wificonnectdialog.h"
#ifdef EMBED_LIBCONNMANQT
#include "networkmanager.h"
#else
#include <connman-qt5/networkmanager.h>
#endif
#include <QTimer>
#include <QDebug>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    auto manager = NetworkManager::instance();
    connect(manager, SIGNAL(technologiesChanged()), SLOT(techChanged()));
    connect(manager, SIGNAL(servicesChanged()), SLOT(servicesChanged()));
    connect(ui->listTech, SIGNAL(currentRowChanged(int)), SLOT(techSelectedChanged()));
    connect(ui->pushSave, SIGNAL(clicked(bool)), SLOT(saveClicked()));
    connect(ui->groupBoxMain, SIGNAL(toggled(bool)), SLOT(groupMainToggled()));
    connect(ui->listWifi, SIGNAL(currentRowChanged(int)), SLOT(onWifiSelected()));
    connect(ui->pushWifiScan, SIGNAL(clicked(bool)), SLOT(scanClicked()));
    connect(ui->pushWifiConnect, SIGNAL(clicked(bool)), SLOT(connectClicked()));
    connect(ui->pushWifiDisconnect, SIGNAL(clicked(bool)), SLOT(disconnectClicked()));
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::techSelectedChanged()
{
    if(ui->listTech->currentItem() == nullptr) return;
    ui->groupBoxMain->setEnabled(true);
    mSelectedTechType = ui->listTech->currentItem()->data(Qt::UserRole).toString();
    if(mCurrentNetworkTech != nullptr)
        mCurrentNetworkTech->disconnect(this);
    mCurrentNetworkTech = mNetworkTechs[ui->listTech->currentRow()];
    connect(mCurrentNetworkTech, SIGNAL(poweredChanged(bool)), SLOT(techPoweredChange(bool)));
    auto services = NetworkManager::instance()->getServices(mSelectedTechType);
    ui->groupBoxMain->setChecked(mCurrentNetworkTech->powered());
    if(!mCurrentNetworkTech->type().compare("wifi")) {
        ui->pushWifiConnect->setEnabled(false);
        ui->pushWifiDisconnect->setEnabled(false);
        ui->groupWifi->show();
        ui->listWifi->clear();
        mNetworkWifiServices.clear();
        for(int i = 0; i < services.size(); i++) {
            mNetworkWifiServices.append(services[i]);
            auto item = new QListWidgetItem(services[i]->name());
            ui->listWifi->addItem(item);
        }
    } else {
        ui->groupWifi->hide();
        if(services.size() > 0) {
            qDebug() << services[0]->ethernet();
            if(mCurrentNetworkService != services[0]) {
                if(mCurrentNetworkService != nullptr) mCurrentNetworkService->disconnect(this);
                mCurrentNetworkService = services[0];
                connect(mCurrentNetworkService, SIGNAL(ipv4Changed(QVariantMap)), SLOT(ipv4Changed(QVariantMap)));
            }
            const QVariantMap &ip = mCurrentNetworkService->ipv4();
            const QStringList &ns = mCurrentNetworkService->nameservers();
            ui->groupIP->setChecked(ip["Method"].toString().compare("dhcp"));
            ui->lineIP->setText(ip["Address"].toString());
            ui->lineNetmask->setText(ip["Netmask"].toString());
            ui->lineGateway->setText(ip["Gateway"].toString());
            if(ns.size() > 0) ui->lineDNS1->setText(ns[0]);
            if(ns.size() > 1) ui->lineDNS2->setText(ns[1]);
            //qDebug() << mCurrentNetworkService->name() << " : " << mCurrentNetworkService->type() ;
        } else {
            mCurrentNetworkService = nullptr;
        }
    }
}

void MainDialog::saveClicked()
{
    if(ui->groupBoxMain->isChecked() != mCurrentNetworkTech->powered())
        mCurrentNetworkTech->setPowered(ui->groupBoxMain->isChecked());
    if(ui->groupBoxMain->isChecked()) {
        QVariantMap ip;
        ip.insert("Method", ui->groupIP->isChecked() ? "manual" : "dhcp");
        if(ui->groupIP->isChecked()) {
            ip.insert("Address", ui->lineIP->text());
            ip.insert("Netmask", ui->lineNetmask->text());
            ip.insert("Gateway", ui->lineGateway->text());
        }
        mCurrentNetworkService->setIpv4Config(ip);
        if(ui->groupDNS->isChecked()) {
            QStringList ns;
            if(!ui->lineDNS1->text().isEmpty()) ns << ui->lineDNS1->text();
            if(!ui->lineDNS2->text().isEmpty()) ns << ui->lineDNS2->text();
            mCurrentNetworkService->setNameserversConfig(ns);
        }
    }
}

void MainDialog::groupMainToggled()
{
    if(mCurrentNetworkService != nullptr) mCurrentNetworkService->disconnect(this);
    if(ui->groupBoxMain->isChecked() != mCurrentNetworkTech->powered())
        mCurrentNetworkTech->setPowered(ui->groupBoxMain->isChecked());
    mCurrentNetworkService = nullptr;
    ui->groupIP->setChecked(false);
    ui->groupDNS->setChecked(false);
}

void MainDialog::techChanged()
{
    auto tech = NetworkManager::instance()->getTechnologies();
    if(mCurrentNetworkTech != nullptr)
        mCurrentNetworkTech->disconnect(this);
    mCurrentNetworkTech = nullptr;
    mSelectedTechType = "";
    mNetworkTechs.clear();
    ui->listTech->blockSignals(true);
    ui->listTech->clear();
    ui->groupBoxMain->setEnabled(false);
    for(int i = 0; i < tech.size(); i++) {
        mNetworkTechs.append(tech[i]);
        auto item = new QListWidgetItem(tech[i]->name());
        item->setData(Qt::UserRole, tech[i]->type());
        ui->listTech->addItem(item);
        if(!mLastSelectedTech.isEmpty() && !mLastSelectedTech.compare(tech[i]->name()))
            ui->listTech->setCurrentItem(item);
    }
    ui->listTech->blockSignals(false);
}

void MainDialog::servicesChanged()
{
    techSelectedChanged();
}

void MainDialog::techPoweredChange(bool /*value*/)
{
    techSelectedChanged();
}

void MainDialog::ipv4Changed(const QVariantMap &)
{
    techSelectedChanged();
}

void MainDialog::scanFinished()
{
}

void MainDialog::onWifiSelected()
{
    int row = ui->listWifi->currentRow();
    if(row < 0) return;
    ui->pushWifiDisconnect->setEnabled(mNetworkWifiServices[row]->connected());
    ui->pushWifiConnect->setEnabled(!mNetworkWifiServices[row]->connected());
}

void MainDialog::scanClicked()
{
    mCurrentNetworkTech->scan();
}

void MainDialog::connectClicked()
{
    int row = ui->listWifi->currentRow();
    WifiConnectDialog dialog(mNetworkWifiServices[row], this);
    dialog.exec();
}

void MainDialog::disconnectClicked()
{
    int row = ui->listWifi->currentRow();
    mNetworkWifiServices[row]->requestDisconnect();
}

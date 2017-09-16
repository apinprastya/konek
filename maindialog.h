#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QList>

namespace Ui {
class MainDialog;
}

class NetworkTechnology;
class NetworkService;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    Ui::MainDialog *ui;
    NetworkTechnology *mCurrentNetworkTech = nullptr;
    NetworkService *mCurrentNetworkService = nullptr;
    QList<NetworkTechnology*> mNetworkTechs;
    QString mLastSelectedTech;
    QString mSelectedTechType;

private slots:
    void techSelectedChanged();
    void saveClicked();
    void groupMainToggled();
    void techChanged();
    void servicesChanged();
    void techPoweredChange(bool /*value*/);
    void ipv4Changed(const QVariantMap &);
};

#endif // MAINDIALOG_H

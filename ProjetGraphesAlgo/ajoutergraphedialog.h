// ajoutergraphedialog.h
#ifndef AJOUTERGRAPHEDIALOG_H
#define AJOUTERGRAPHEDIALOG_H

#include <QDialog>

namespace Ui {
class AjouterGrapheDialog;
}

class AjouterGrapheDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterGrapheDialog(QWidget *parent = nullptr);
    ~AjouterGrapheDialog();

    QString getNom() const;
    bool estValue() const;
    bool estOriente() const;

    bool fichierCharge() const;
    QString getCheminFichier() const;

private slots:
    void on_buttonChargerDepuisFichier_clicked();

private:
    Ui::AjouterGrapheDialog *ui;
    bool aChargeDepuisFichier = false;
    QString cheminFichier;


};

#endif // AJOUTERGRAPHEDIALOG_H

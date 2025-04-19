#include "ajoutergraphedialog.h"
#include "ui_ajoutergraphedialog.h"
#include <QFileDialog>

AjouterGrapheDialog::AjouterGrapheDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjouterGrapheDialog)
{
    ui->setupUi(this);
}

AjouterGrapheDialog::~AjouterGrapheDialog()
{
    delete ui;
}

QString AjouterGrapheDialog::getNom() const {
    return ui->lineNomGraphe->text();
}

bool AjouterGrapheDialog::estValue() const {
    return ui->checkValue->isChecked();
}

bool AjouterGrapheDialog::estOriente() const {
    return ui->checkOriente->isChecked();
}



void AjouterGrapheDialog::on_buttonChargerDepuisFichier_clicked()
{
    QString chemin = QFileDialog::getOpenFileName(this, "Charger un fichier", "", "Fichiers texte (*.txt *.dat)");
    if (!chemin.isEmpty()) {
        aChargeDepuisFichier = true;
        cheminFichier = chemin;
        accept();
    }
}

bool AjouterGrapheDialog::fichierCharge() const {
    return aChargeDepuisFichier;
}

QString AjouterGrapheDialog::getCheminFichier() const {
    return cheminFichier;
}




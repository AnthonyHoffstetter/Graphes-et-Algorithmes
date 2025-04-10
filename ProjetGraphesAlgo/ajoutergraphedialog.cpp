#include "ajoutergraphedialog.h"
#include "ui_ajoutergraphedialog.h"

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


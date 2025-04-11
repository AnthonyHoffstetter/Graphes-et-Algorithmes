// ajouterarcsdialog.cpp
#include "ajouterarcsdialog.h"
#include "graphevalue.h"
#include "ui_ajouterarcsdialog.h"
#include <QMessageBox>

AjouterArcsDialog::AjouterArcsDialog(Graphe* graphe, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjouterArcsDialog),
    g(graphe)
{
    ui->setupUi(this);
    if (dynamic_cast<GrapheValue*>(g)) {
        ui->spinBoxPoids->show();
        ui->labelPoids->show(); // si tu as un QLabel associé au poids
    } else {
        ui->spinBoxPoids->hide();
        ui->labelPoids->hide();
    }


}

int AjouterArcsDialog::getDepart()  {
    return ui->comboBoxDepart->currentText().toInt();
}

int AjouterArcsDialog::getArrivee()  {
    return ui->comboBoxArrivee->currentText().toInt();
}

int AjouterArcsDialog::getPoids()  {
    return ui->spinBoxPoids->value();
}


AjouterArcsDialog::~AjouterArcsDialog()
{
    delete ui;
}

void AjouterArcsDialog::on_boutonAjouter_clicked()
{

}

void AjouterArcsDialog::on_buttonAjouterSommet_clicked()
{
    g->ajouterSommet();

    int nouveauSommet = g->aps[0];

    QString label = QString::number(nouveauSommet);
    ui->comboBoxDepart->addItem(label);
    ui->comboBoxArrivee->addItem(label);

    //QMessageBox::information(this, "Succès", QString("Sommet %1 ajouté avec succès.").arg(nouveauSommet));
}




void AjouterArcsDialog::on_BouttonAjouterArc_clicked()
{
    int u = getDepart();
    int v = getArrivee();

    if (g->ArcDejaPresent(u, v)) {
        QMessageBox::information(this, "Info", "Cet arc existe déjà.");
        return;
    }

    // Si le graphe est pondéré → cast dynamique
    if (auto gv = dynamic_cast<GrapheValue*>(g)) {
        int p = getPoids();
        gv->ajouterArc(u, v, p);
        ui->listeArcs->addItem(QString("%1 → %2 (poids %3)").arg(u).arg(v).arg(p));
    } else {
        g->ajouterArc(u, v);
        ui->listeArcs->addItem(QString("%1 → %2").arg(u).arg(v));
    }



    //QMessageBox::information(this, "Succès", "Arc ajouté avec succès.");
}



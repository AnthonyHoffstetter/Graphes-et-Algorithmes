#include "dialogordonnancement.h"
#include "dialogtachesprecedentes.h"
#include "ui_dialogordonnancement.h"

DialogOrdonnancement::DialogOrdonnancement(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogOrdonnancement)
{
    ui->setupUi(this);
}

DialogOrdonnancement::~DialogOrdonnancement()
{
    delete ui;
}

void DialogOrdonnancement::on_buttonAddTache_clicked() {
    QString nom = ui->lineEditTacheNom->text();
    int duree = ui->spinBoxDuree->value();

    if (nom.isEmpty()) return;

    Tache t; // Déclaration hors des blocs

    t.nom = nom;
    t.duree = duree;

    if (listeTaches.count() == 0) {
        t.precedents = QList<int>{}; // Pas de précédents
    } else {
        DialogTachesPrecedentes dp(listeTaches, this);
        if (dp.exec() == QDialog::Accepted) {
            t.precedents = dp.getTachesPrecedentes();
        } else {
            return; // si l'utilisateur annule, on ne fait rien
        }
    }

    listeTaches.append(t);
    ajouterTacheDansTable(t);
}

QList<Tache> DialogOrdonnancement::getListeTaches() const {
    return listeTaches;
}



void DialogOrdonnancement::ajouterTacheDansTable(const Tache& tache)
{
    int row = ui->listTaches->rowCount();
    ui->listTaches->insertRow(row);

    // Colonne 0 : numéro de la tâche
    ui->listTaches->setItem(row, 0, new QTableWidgetItem(QString::number(row)));

    // Colonne 1 : nom
    ui->listTaches->setItem(row, 1, new QTableWidgetItem(tache.nom));

    // Colonne 2 : durée
    ui->listTaches->setItem(row, 2, new QTableWidgetItem(QString::number(tache.duree)));

    // Colonne 3 : indices des tâches précédentes (affichés comme chaîne)
    QStringList precedentsStr;
    for (int indice : tache.precedents)
        precedentsStr << QString::number(indice);

    ui->listTaches->setItem(row, 3, new QTableWidgetItem(precedentsStr.join(", ")));
}


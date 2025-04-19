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

    Tache t;

    t.nom = nom;
    t.duree = duree;

    if (listeTaches.count() == 0) {
        t.precedents = QList<int>{};
    } else {
        DialogTachesPrecedentes dp(listeTaches, this);
        if (dp.exec() == QDialog::Accepted) {
            t.precedents = dp.getTachesPrecedentes();
        } else {
            return;
        }
    }

    listeTaches.append(t);
    ajouterTacheDansTable(t);
    ui->lineEditTacheNom->clear();
    ui->spinBoxDuree->setValue(0);
}

QList<Tache> DialogOrdonnancement::getListeTaches() const {
    return listeTaches;
}



void DialogOrdonnancement::ajouterTacheDansTable(const Tache& tache)
{
    int row = ui->listTaches->rowCount();
    ui->listTaches->insertRow(row);

    // Colonne 0 : nom
    ui->listTaches->setItem(row, 0, new QTableWidgetItem(tache.nom));

    // Colonne 1 : durée
    ui->listTaches->setItem(row, 1, new QTableWidgetItem(QString::number(tache.duree)));

    // Colonne 2 : indices des tâches précédentes
    QStringList precedentsStr;
    for (int indice : tache.precedents)
        precedentsStr << QString::number(indice);

    ui->listTaches->setItem(row, 2, new QTableWidgetItem(precedentsStr.join(", ")));
}


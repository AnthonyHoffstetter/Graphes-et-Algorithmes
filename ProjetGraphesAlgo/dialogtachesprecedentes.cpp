#include "dialogtachesprecedentes.h"
#include "qlistwidget.h"
#include "ui_dialogtachesprecedentes.h"

DialogTachesPrecedentes::DialogTachesPrecedentes(const QList<Tache> &tachesExistantes,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogTachesPrecedentes)
{
    ui->setupUi(this);

    for (int i = 0; i < tachesExistantes.size(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(tachesExistantes[i].nom);
        item->setData(Qt::UserRole, i);
        item->setCheckState(Qt::Unchecked);
        ui->listTachesAnterieures->addItem(item);
    }
}

DialogTachesPrecedentes::~DialogTachesPrecedentes()
{
    delete ui;
}

QList<int> DialogTachesPrecedentes::getTachesPrecedentes() const {
    QList<int> selection;

    for (int i = 0; i < ui->listTachesAnterieures->count(); ++i) {
        QListWidgetItem *item = ui->listTachesAnterieures->item(i);
        if (item->checkState() == Qt::Checked) {
            selection.append(item->data(Qt::UserRole).toInt());
        }
    }
    return selection;
}


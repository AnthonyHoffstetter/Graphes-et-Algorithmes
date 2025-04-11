// ajouterarcsdialog.h
#ifndef AJOUTERARCSDIALOG_H
#define AJOUTERARCSDIALOG_H

#include <QDialog>
#include "graphe.h"

namespace Ui {
class AjouterArcsDialog;
}

class AjouterArcsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterArcsDialog(Graphe* graphe, QWidget *parent = nullptr);
    ~AjouterArcsDialog();

    int getDepart();
    int getArrivee();
    int getPoids();

private slots:
    void on_boutonAjouter_clicked();

    void on_buttonAjouterSommet_clicked();

    void on_BouttonAjouterArc_clicked();

private:
    Ui::AjouterArcsDialog *ui;
    Graphe* g;
};

#endif // AJOUTERARCSDIALOG_H

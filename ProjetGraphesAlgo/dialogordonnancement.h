#ifndef DIALOGORDONNANCEMENT_H
#define DIALOGORDONNANCEMENT_H

#include <QDialog>



namespace Ui {
class DialogOrdonnancement;
}

struct Tache {
    QString nom;
    int duree;
    QList<int> precedents; // indices des tâches précédentes
};

class DialogOrdonnancement : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrdonnancement(QWidget *parent = nullptr);
    ~DialogOrdonnancement();
    void ajouterTacheDansTable(const Tache& tache);
    QList<Tache> getListeTaches() const;


private:
    Ui::DialogOrdonnancement *ui;

    QList<Tache> listeTaches;

private slots:
    void on_buttonAddTache_clicked();
};

#endif // DIALOGORDONNANCEMENT_H

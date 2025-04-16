#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphe.h"
#include "graphevalue.h"
#include "qgraphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mettreAJourListeGraphes();
    void afficherListeGraphes();
    void mettreAJourComboGraphes();
    void afficherGraphe(QString nom, QGraphicsView* vue);
    void afficherGrapheAccueil(QString nom);
    void mettreAJourComboGrapheAlgo();
    void remplirListeAlgorithmes();
    void remplirListeAlgorithmes2();
    void MettreAJourExplications();
    void trouverCheminsCritiques(GrapheValue& g, int courant, int fin, QMap<int, int>& marges,
                                 QList<int>& cheminActuel, int dureeActuelle,
                                 QList<QPair<QString, int>>& chemins);




private:
    Ui::MainWindow *ui;
    std::map<QString, Graphe*> graphes;
private slots:
    void on_actionAjouter_un_graphe_triggered();
    void on_actionListe_des_graphes_triggered();
    void on_actionAccueil_triggered();
    void on_actionExecuter_un_algorithme_triggered();
    void on_buttonLancerAlgo_clicked();
    void on_actionExplications_triggered();
    void on_actionOrdonnancement_triggered();
    void on_buttonOrdonnancementStart_clicked();
};
#endif // MAINWINDOW_H

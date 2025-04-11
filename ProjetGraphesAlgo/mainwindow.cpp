#include "mainwindow.h"
#include "graphevalue.h"
#include "ui_mainwindow.h"
#include "ajoutergraphedialog.h"
#include "ajouterarcsdialog.h"
#include <QMessageBox>
#include <QGraphicsTextItem>
#include "algorithms.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->comboGraphesAccueil, &QComboBox::currentTextChanged,
            this, &MainWindow::afficherGraphe);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mettreAJourListeGraphes()
{
    ui->tableGraphes->setRowCount(0); // Vider le tableau

    for (const auto& [nom, g] : graphes) {
        int row = ui->tableGraphes->rowCount();
        ui->tableGraphes->insertRow(row);

        // Déterminer si valué
        bool value = dynamic_cast<GrapheValue*>(g) != nullptr;

        // Infos
        int n = g->aps[0]; // Nb sommets
        int m = g->fs[0]-n;  // Nb arêtes

        ui->tableGraphes->setItem(row, 0, new QTableWidgetItem(nom));
        ui->tableGraphes->setItem(row, 1, new QTableWidgetItem(QString::number(n)));
        ui->tableGraphes->setItem(row, 2, new QTableWidgetItem(QString::number(m)));
        ui->tableGraphes->setItem(row, 3, new QTableWidgetItem(g->estOriente ? "true" : "false"));
        ui->tableGraphes->setItem(row, 4, new QTableWidgetItem(value ? "true" : "false"));
    }
}

void MainWindow::mettreAJourComboGraphes()
{
    ui->comboGraphesAccueil->clear();
    for (const auto& [nom, _] : graphes) {
        ui->comboGraphesAccueil->addItem(nom);
    }
}


void MainWindow::afficherListeGraphes()
{
    mettreAJourListeGraphes();
    ui->stackedWidget->setCurrentIndex(1); // Page 1 = liste des graphes
}

void MainWindow::on_actionListe_des_graphes_triggered()
{
    afficherListeGraphes();
}

void MainWindow::on_actionAccueil_triggered(){
    ui->stackedWidget->setCurrentIndex(0);
    mettreAJourComboGraphes();
}




void MainWindow::on_actionAjouter_un_graphe_triggered()
{
    AjouterGrapheDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {

        QString nom = dialog.getNom();
        bool oriente = dialog.estOriente();
        bool value = dialog.estValue();

        // Vérifie unicité
        if (graphes.find(nom) != graphes.end()) {
            QMessageBox::warning(this, "Erreur", "Un graphe avec ce nom existe déjà !");
            return;
        }

        // Cas 1 : l'utilisateur a choisi de charger depuis un fichier
        if (dialog.fichierCharge()) {
            QString chemin = dialog.getCheminFichier();

            Graphe* g = value ? static_cast<Graphe*>(new GrapheValue(oriente)) : new Graphe(oriente);
            g->saisirDepuisFichier(chemin.toStdString());

            graphes[nom] = g;
            mettreAJourListeGraphes();
            mettreAJourComboGraphes();
            return;
        }

        // Cas 2 : ajout manuel via interface
        Graphe* g = value ? static_cast<Graphe*>(new GrapheValue(oriente)) : new Graphe(oriente);

        AjouterArcsDialog arcsDialog(g, this);
        if (arcsDialog.exec() == QDialog::Accepted) {
            graphes[nom] = g;
            mettreAJourListeGraphes();
            mettreAJourComboGraphes();
            mettreAJourComboGrapheAlgo();
        } else {
            delete g;
        }
    }
}




void MainWindow::afficherGraphe(QString nom)
{
    if (graphes.find(nom) == graphes.end()) return;

    Graphe* g = graphes[nom];
    bool oriente = g->estOriente;
    bool value = dynamic_cast<GrapheValue*>(g) != nullptr;

    QGraphicsScene* scene = new QGraphicsScene(this);
    int n = g->aps[0];
    std::vector<QPointF> positions(n + 1);

    // Positionner les sommets
    for (int i = 1; i <= n; ++i) {
        int x = 150 + (i * 120) % 500;
        int y = 150 + ((i * 170) % 300);
        positions[i] = QPointF(x + 20, y + 20);
        scene->addEllipse(x, y, 40, 40);
        scene->addText(QString::number(i))->setPos(x + 10, y + 5);
    }

    // Dessiner les arcs
    for (int u = 1; u <= n; ++u) {
        for (int i = g->aps[u]; g->fs[i] != 0; ++i) {
            int v = g->fs[i];
            QPointF p1 = positions[u];
            QPointF p2 = positions[v];

            if (u == v) {
                // Arc boucle stylisé (vers soi-même)
                QPainterPath loopPath;
                loopPath.moveTo(p1);
                loopPath.cubicTo(QPointF(p1.x() + 30, p1.y() - 60),
                                 QPointF(p1.x() - 30, p1.y() - 60),
                                 p1);

                scene->addPath(loopPath);

                if (oriente) {
                    QPointF tip = p1;
                    double angle = -M_PI_2; // vers le haut
                    QPointF arrowP1 = tip - QPointF(std::cos(angle + M_PI / 6) * 10, std::sin(angle + M_PI / 6) * 10);
                    QPointF arrowP2 = tip - QPointF(std::cos(angle - M_PI / 6) * 10, std::sin(angle - M_PI / 6) * 10);
                    scene->addLine(QLineF(tip, arrowP1));
                    scene->addLine(QLineF(tip, arrowP2));
                }

                if (value) {
                    GrapheValue* gv = dynamic_cast<GrapheValue*>(g);
                    Sommet* s = gv->trouverSommet(u);
                    if (s && s->fileSucesseur) {
                        int poids = s->fileSucesseur->poids;
                        scene->addText(QString::number(poids))->setPos(p1.x(), p1.y() - 70);
                    }
                }
                continue;
            }

            // Calcul de direction et ajustement pour bord du cercle
            QLineF baseLine(p1, p2);
            QLineF shiftedLine = baseLine;
            shiftedLine.setP1(baseLine.pointAt(20.0 / baseLine.length()));
            shiftedLine.setP2(baseLine.pointAt(1.0 - 20.0 / baseLine.length()));

            QPainterPath path;
            QPointF ctrl = (shiftedLine.p1() + shiftedLine.p2()) / 2 + QPointF(0, -30); // point de courbure
            path.moveTo(shiftedLine.p1());
            path.quadTo(ctrl, shiftedLine.p2());
            scene->addPath(path);

            // Ajouter flèche
            if (oriente) {
                double angle = std::atan2(-baseLine.dy(), baseLine.dx());
                QPointF tip = shiftedLine.p2();
                QPointF arrowP1 = tip - QPointF(std::cos(angle + M_PI / 6) * 10, -std::sin(angle + M_PI / 6) * 10);
                QPointF arrowP2 = tip - QPointF(std::cos(angle - M_PI / 6) * 10, -std::sin(angle - M_PI / 6) * 10);

                scene->addLine(QLineF(tip, arrowP1));
                scene->addLine(QLineF(tip, arrowP2));
            }

            // Ajouter le poids si GrapheValue
            if (value) {
                GrapheValue* gv = dynamic_cast<GrapheValue*>(g);
                Sommet* s = gv->trouverSommet(u);
                if (s) {
                    Successeur* suc = s->fileSucesseur;
                    while (suc) {
                        if (suc->sommet->id == v) {
                            int poids = suc->poids;
                            QPointF textPos = (shiftedLine.p1() + shiftedLine.p2()) / 2 + QPointF(0, -15);
                            scene->addText(QString::number(poids))->setPos(textPos);
                            break;
                        }
                        suc = suc->successeurSuivant;
                    }
                }
            }
        }
    }

    ui->graphicsView->setScene(scene);
}

void MainWindow::mettreAJourComboGrapheAlgo()
{
    ui->comboGrapheAlgo->clear();
    for (const auto& [nom, _] : graphes) {
        ui->comboGrapheAlgo->addItem(nom);
    }
}

void MainWindow::remplirListeAlgorithmes()
{
    ui->comboAlgo->clear();
    ui->comboAlgo->addItem("Calcul des distances");
    ui->comboAlgo->addItem("Rangs des sommets");
    ui->comboAlgo->addItem("Tarjan");
    ui->comboAlgo->addItem("Points d'articulation et isthmes");
    ui->comboAlgo->addItem("Dijkstra");
    ui->comboAlgo->addItem("Dantzig");
    ui->comboAlgo->addItem("Kruskal");
}



void MainWindow::on_actionExecuter_un_algorithme_triggered()
{
    remplirListeAlgorithmes();
    mettreAJourComboGrapheAlgo();
    ui->stackedWidget->setCurrentIndex(2); // adapte l'index
}






void MainWindow::on_buttonLancerAlgo_clicked()
{
    QString algo = ui->comboAlgo->currentText();
    QString nomGraphe = ui->comboGrapheAlgo->currentText();

    if (graphes.find(nomGraphe) == graphes.end()) {
        ui->resultatAlgo->setPlainText("Graphe introuvable.");
        return;
    }

    Graphe* g = graphes[nomGraphe];
    QString res;

    if (algo == "Calcul des distances") {
        if (dynamic_cast<GrapheValue*>(g)) {
            res = "Le calcul des distances ne fonctionne que sur des graphes non pondérés.\n";
        }
        else{
        std::vector<std::vector<int>> mat = Algorithms::calculerMatriceDistances(*g);
        res = "Matrice des distance :\n\n";
        for (int i = 1; i < mat.size(); ++i) { // ignore la première ligne
            for (int j = 1; j < mat[i].size(); ++j) { // ignore la première colonne
                int d = mat[i][j];
                res += (d == INT_MAX ? "∞" : QString::number(d)) + " ";
            }
            res += "\n";
        }
        }
    }
    else if (algo == "Rangs des sommets") {
        std::vector<int> rangs;
        if (Algorithms::calculRangs(*g, rangs)) {
            res = "Rangs des sommets :\n";
            for (int i = 1; i < rangs.size(); ++i)
                res += QString("Sommet %1 : Rang %2\n").arg(i).arg(rangs[i]);
        } else {
            res = "Circuit détecté, pas de rangs.";
        }
    }
    else if (algo == "Tarjan") {
        if(!g->estOriente)
            res = "Algorithme impossible sur graphe non orienté";
        else{
        std::vector<int> cfc;
        int nbCfc = 0;
        Algorithms::tarjan(*g, cfc, nbCfc);

        res = "\nSommets par composante :\n";
        for (int i = 1; i <= nbCfc; ++i) {
            res += "CFC " + QString::number(i) + " : ";
            for (int j = 1; j < cfc.size(); ++j) {
                if (cfc[j] == i)
                    res += QString::number(j) + " ";
            }
            res += "\n";
        }

        auto GR = Algorithms::construireGrapheReduit(*g, cfc, nbCfc);

        res += "\nGraphe reduit :\n";

        for (int u = 1; u <= nbCfc; ++u) {
            res += "CFC " + QString::number(u) + " -> ";
            if (GR[u].empty()) {
                res += "(aucun successeur)";
            } else {
                for (int v : GR[u]) {
                    res += QString::number(v) + " ";
                }
            }
            res += "\n";
        }

        res += "\nBases (CFC sans predecesseur) : ";
        std::vector<int> ddi(nbCfc + 1, 0);

        for (int u = 1; u <= nbCfc; ++u) {
            for (int v : GR[u]) {
                ddi[v]++;
            }
        }

        for (int i = 1; i <= nbCfc; ++i) {
            if (ddi[i] == 0) {
                res += QString::number(i) + " ";
            }
        }

        // Graphe réduit à sauvegarder
        QString nomReduit = nomGraphe + "_reduit";
        Graphe* reduit = new Graphe(g->estOriente);
        for (int i = 1; i <= nbCfc; ++i) reduit->ajouterSommet();
        for (int u = 1; u <= nbCfc; ++u) {
            for (int v : GR[u]) {
                reduit->ajouterArc(u, v);
            }
        }
        graphes[nomReduit] = reduit;
        res += "\n\nGraphe réduit sauvegardé";
        mettreAJourListeGraphes();
        mettreAJourComboGraphes();
        mettreAJourComboGrapheAlgo();
        }
    }
    else if (algo == "Points d'articulation et isthmes") {
        if(g->estOriente)
            res = "Algorithme impossible sur graphe orienté";
        else{
        std::vector<int> points;
        std::vector<std::pair<int, int>> isthmes;
        Algorithms::articulationsEtIsthmes(*g,points,isthmes);

        res += "Points d'articulation : ";
        for (int p : points) res += QString::number(p) + " ";
        res += "\nIsthmes : \n";
        for (auto [u, v] : isthmes)
            res += QString("%1 - %2\n").arg(u).arg(v);
        }
    }
    else if (algo == "Dijkstra") {
        GrapheValue* gv = dynamic_cast<GrapheValue*>(g);
        if (!gv) {
            res = "Dijkstra s'applique uniquement sur des graphes pondérés.";
        } else {
            int source = 1; // par défaut sommet 1
            std::vector<int> dist = Algorithms::dijkstra(*gv, source);
            res = "Distances depuis le sommet " + QString::number(source) + " :\n";
            for (int i = 1; i < dist.size(); ++i) {
                res += QString("%1 → %2 : %3\n").arg(source).arg(i).arg(
                    dist[i] == INT_MAX ? "∞" : QString::number(dist[i]));
            }
        }
    }
    else if (algo == "Dantzig") {
        GrapheValue* gv = dynamic_cast<GrapheValue*>(g);
        if (!gv) {
            res = "Dantzig (Floyd-Warshall) s'applique uniquement sur des graphes pondérés.";
        } else {
            try {
                std::vector<std::vector<int>> mat = Algorithms::dantzig(*gv);
                res = "Matrice des plus courts chemins (Dantzig) :\n\n";
                for (int i = 1; i < mat.size(); ++i) {
                    for (int j = 1; j < mat[i].size(); ++j) {
                        int d = mat[i][j];
                        res += (d == INT_MAX ? "∞" : QString::number(d)) + " ";
                    }
                    res += "\n";
                }
            } catch (const std::runtime_error& e) {
                res = QString("Erreur : %1").arg(e.what());
            }

        }
    }
    else if (algo == "Kruskal") {
        GrapheValue* gv = dynamic_cast<GrapheValue*>(g);
        if (!gv || gv->estOriente) {
            res = "Kruskal s'applique uniquement sur des graphes non orientés pondérés.";
        } else {
            try {
                auto arbre = Algorithms::kruskal(*gv);
                int poidsTotal = 0;
                res = "Arbre recouvrant minimal (Kruskal) :\n\n";
                for (auto& [u, v, p] : arbre) {
                    res += QString("%1 - %2 (poids: %3)\n").arg(u).arg(v).arg(p);
                    poidsTotal += p;
                }
                res += QString("\nPoids total : %1").arg(poidsTotal);
            } catch (const std::exception& e) {
                res = QString("Erreur : %1").arg(e.what());
            }
        }
    }


    else {
        res = "Algorithme non pris en charge.";
    }

    ui->resultatAlgo->setPlainText(res);
}


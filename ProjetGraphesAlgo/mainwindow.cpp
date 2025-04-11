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
    ui->comboAlgo->addItem("Tarjan (CFC)");
    ui->comboAlgo->addItem("Points d'articulation et isthmes");
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
    else if (algo == "Rangs des sommets") {
        std::vector<int> rangs;
        if (Algorithms::calculRangs(*g, rangs)) {
            res = "Rangs des sommets :\n";
            for (int i = 0; i < rangs.size(); ++i)
                res += QString("Sommet %1 : Rang %2\n").arg(i + 1).arg(rangs[i]);
        } else {
            res = "Circuit détecté, pas de rangs.";
        }
    }
    else if (algo == "Tarjan (CFC)") {
        std::vector<int> cfc;
        int nbCfc = 0;
        Algorithms::tarjan(*g, cfc, nbCfc);
        std::vector<std::vector<int>> GR = Algorithms::construireGrapheReduit(*g, cfc, nbCfc);

        // Ajouter le graphe réduit à la map
        QString nomReduit = nomGraphe + "_reduit";
        Graphe* reduit = new Graphe(g->estOriente);
        for (int i = 1; i <= nbCfc; ++i) reduit->ajouterSommet();
        for (int u = 0; u < GR.size(); ++u) {
            for (int v : GR[u]) {
                reduit->ajouterArc(u + 1, v + 1);
            }
        }
        graphes[nomReduit] = reduit;
        mettreAJourListeGraphes();
        mettreAJourComboGraphes();

        // Affichage du résultat dans le champ texte
        QString texte;
        QTextStream stream(&texte);
        stream << "Graphe réduit ajouté sous le nom : " << nomReduit << "\n";
        stream << "Graphe réduit :\n";
        for (int i = 0; i < GR.size(); ++i) {
            stream << "Composante " << i + 1 << " : ";
            for (int v : GR[i])
                stream << v + 1 << " ";
            stream << "\n";
        }

        stream << "\nBases du graphe réduit :\n";
        for (int i = 0; i < GR.size(); ++i) {
            if (GR[i].empty()) {
                stream << "CFC " << i + 1 << "\n";
            }
        }

        res = texte;
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
    else {
        res = "Algorithme non pris en charge.";
    }

    ui->resultatAlgo->setPlainText(res);
}


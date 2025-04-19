#include "mainwindow.h"
#include "dialogordonnancement.h"
#include "graphevalue.h"
#include "ui_mainwindow.h"
#include "ajoutergraphedialog.h"
#include "ajouterarcsdialog.h"
#include <QMessageBox>
#include <QGraphicsTextItem>
#include "algorithms.h"
#include <QQueue>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->comboGraphesAccueil, &QComboBox::currentTextChanged,
            this, &MainWindow::afficherGrapheAccueil);
    connect(ui->comboAlgo_2, &QComboBox::currentTextChanged,
            this, &MainWindow::MettreAJourExplications);
    mettreAJourComboGraphes();


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

void MainWindow::on_buttonSauvegarderGraphe_clicked()
{
    QString nomGraphe = ui->comboGrapheAlgo->currentText();

    if (graphes.find(nomGraphe) == graphes.end()) {
        QMessageBox::warning(this, "Erreur", "Aucun graphe sélectionné.");
        return;
    }

    QString chemin = QFileDialog::getSaveFileName(this, "Enregistrer le graphe", nomGraphe + ".txt", "Fichiers texte (*.txt)");
    if (chemin.isEmpty()) return;

    Graphe* g = graphes[nomGraphe];
    g->sauvegarderDansFichier(chemin.toStdString());

    QMessageBox::information(this, "Succès", "Graphe sauvegardé !");
}


void MainWindow::afficherGrapheAccueil(QString nom)
{
    afficherGraphe(nom, ui->graphicsView);
}


void MainWindow::mettreAJourComboGraphes()
{
    ui->comboGraphesAccueil->clear();
    for (const auto& [nom, _] : graphes) {
        ui->comboGraphesAccueil->addItem(nom);
    }

    bool vide = graphes.empty();
    ui->buttonSauvegarderGraphe->setVisible(!vide);
}


void MainWindow::afficherListeGraphes()
{
    mettreAJourListeGraphes();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionListe_des_graphes_triggered()
{
    afficherListeGraphes();
}




void MainWindow::on_actionAccueil_triggered(){
    ui->stackedWidget->setCurrentIndex(0);
    mettreAJourComboGraphes();
}

void MainWindow::on_actionOrdonnancement_triggered(){
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::trouverCheminsCritiques(GrapheValue& g, int courant, int fin, QMap<int, int>& marges,
                             QList<int>& cheminActuel, int dureeActuelle, QList<QPair<QString, int>>& chemins) {
    if (marges[courant] != 0) return;

    cheminActuel.append(courant);

    if (courant == fin) {
        QString cheminStr;
        for (int i = 0; i < cheminActuel.size(); ++i) {
            if (i > 0) cheminStr += " → ";
            cheminStr += QString::number(cheminActuel[i]);
        }
        chemins.append({cheminStr, dureeActuelle});
    } else {
        Sommet* s = g.trouverSommet(courant);
        for (Successeur* succ = s->fileSucesseur; succ; succ = succ->successeurSuivant) {
            int suivant = succ->sommet->id;
            if (marges[suivant] == 0)
                trouverCheminsCritiques(g, suivant, fin, marges, cheminActuel, dureeActuelle + succ->poids, chemins);
        }
    }

    cheminActuel.removeLast();
}

void MainWindow::on_buttonOrdonnancementStart_clicked() {
    DialogOrdonnancement dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QList<Tache> taches = dialog.getListeTaches();

        int nbTaches = taches.size();
        GrapheValue g;

        for(int i = 0; i < nbTaches + 2; ++i){
            g.ajouterSommet();
        }

        int id_DT = 1;
        int id_FT = nbTaches + 2;

        // Étape 1 : relier DT aux tâches sans prédécesseurs
        for (int i = 0; i < nbTaches; ++i) {
            if (taches[i].precedents.isEmpty()) {
                g.ajouterArc(id_DT, i + 2, 0); // i+2 = ID de la tâche i

            }
        }

        // Étape 2 : relier les tâches entre elles
        for (int i = 0; i < nbTaches; ++i) {
            for (int pred : taches[i].precedents) {
                g.ajouterArc(pred + 2, i + 2, taches[pred].duree);

            }
        }

        // Étape 3 : relier les tâches sans successeurs à FT
        QSet<int> tachesAvecSuccesseurs;
        for (int i = 0; i < nbTaches; ++i) {
            for (int pred : taches[i].precedents) {
                tachesAvecSuccesseurs.insert(pred);
            }
        }
        for (int i = 0; i < nbTaches; ++i) {
            if (!tachesAvecSuccesseurs.contains(i)) {
                g.ajouterArc(i + 2, id_FT, taches[i].duree);

            }
        }


        // Étape 4 : Sauvegarder dans listeDeGraphe
        graphes["Ordonnancement"] = new GrapheValue(g);


        // Étape 5 : Affichage du graphe dans graphicsView
        afficherGraphe("Ordonnancement",ui->graphicsView_Ordonnancement);

        // Étape 6 : Affichage de la liste des tâches à droite
        ui->listWidgetTachesetId->clear();
        ui->listWidgetTachesetId->addItem("1 - DT");
        for (int i = 0; i < taches.size(); ++i)
            ui->listWidgetTachesetId->addItem(QString::number(i + 2) + " - " + taches[i].nom); // ✅ i+2
        ui->listWidgetTachesetId->addItem(QString::number(id_FT) + " - FT");

        // Étape 7 : Ordonnancement

        int n = g.aps[0];
        QVector<int> dateTot(n + 1, 0);
        QVector<int> dateTard(n + 1, 0);
        QMap<int, int> marges;
        for (int i = 1; i <= n; ++i)
            marges[i] = dateTard[i] - dateTot[i];

        QVector<int> degreEntrant(n + 1, 0);
        QVector<int> ordreTopo;

        // === Étape 7.1 : calcul des degrés entrants
        for (int u = 1; u <= n; ++u) {
            Sommet* sommet = g.trouverSommet(u);
            if (!sommet) continue;

            for (Successeur* succ = sommet->fileSucesseur; succ != nullptr; succ = succ->successeurSuivant) {
                int v = succ->sommet->id;
                degreEntrant[v]++;
            }
        }

        // === Étape 7.2 : tri
        QQueue<int> file;
        for (int i = 1; i <= n; ++i)
            if (degreEntrant[i] == 0)
                file.enqueue(i);

        while (!file.isEmpty()) {
            int u = file.dequeue();
            ordreTopo.append(u);

            Sommet* sommet = g.trouverSommet(u);
            if (!sommet) continue;

            for (Successeur* succ = sommet->fileSucesseur; succ != nullptr; succ = succ->successeurSuivant) {
                int v = succ->sommet->id;
                int poids = succ->poids;

                degreEntrant[v]--;
                if (degreEntrant[v] == 0)
                    file.enqueue(v);

                dateTot[v] = std::max(dateTot[v], dateTot[u] + poids);
            }
        }

        // === Étape 7.3 : dates au plus tard
        dateTard[id_FT] = dateTot[id_FT];
        for (int i = ordreTopo.size() - 1; i >= 0; --i) {
            int u = ordreTopo[i];
            if (u == id_FT) continue;

            Sommet* sommet = g.trouverSommet(u);
            if (!sommet) continue;

            int minTard = INT_MAX;
            for (Successeur* succ = sommet->fileSucesseur; succ != nullptr; succ = succ->successeurSuivant) {
                int v = succ->sommet->id;
                int poids = succ->poids;

                minTard = std::min(minTard, dateTard[v] - poids);
            }

            dateTard[u] = (minTard == INT_MAX) ? dateTot[u] : minTard;
        }

        QList<QPair<QString, int>> cheminsCritiques;
        QList<int> cheminActuel;
        trouverCheminsCritiques(g, id_DT, id_FT, marges, cheminActuel, 0, cheminsCritiques);

        // Garde uniquement ceux avec durée max
        int dureeMax = dateTot[id_FT];
        QList<QString> cheminsFinal;

        for (auto pair : cheminsCritiques) {
            if (pair.second == dureeMax)
                cheminsFinal.append(pair.first);
        }

        // Convertir les cheminsFinal en noms
        QStringList cheminsConvertis;
        for (const QString& chemin : cheminsFinal) {
            QStringList ids = chemin.split(" → ");
            QStringList noms;

            for (const QString& idStr : ids) {
                int id = idStr.toInt();

                if (id == 1)
                    noms << "DT";
                else if (id == taches.size() + 2)
                    noms << "FT";
                else
                    noms << taches[id - 2].nom;
            }

            cheminsConvertis << "Chemin critique : " + noms.join(" → ");
        }

        QString resultat = cheminsConvertis.join("\n\n");
        resultat += QString("\n\nDurée totale : %1").arg(dureeMax);

        ui->TextEditOrdonnancementResultat->setPlainText(resultat);


        mettreAJourListeGraphes();
        mettreAJourComboGraphes();
        mettreAJourComboGrapheAlgo();
    }
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




void MainWindow::afficherGraphe(QString nom,QGraphicsView *vue)
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

    vue->setScene(scene);

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
    ui->comboAlgo->addItem("FS et APS");
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

void MainWindow::remplirListeAlgorithmes2()
{
    ui->comboAlgo_2->clear();
    ui->comboAlgo_2->addItem(" ");
    ui->comboAlgo_2->addItem("Calcul des distances");
    ui->comboAlgo_2->addItem("Rangs des sommets");
    ui->comboAlgo_2->addItem("Tarjan");
    ui->comboAlgo_2->addItem("Points d'articulation et isthmes");
    ui->comboAlgo_2->addItem("Dijkstra");
    ui->comboAlgo_2->addItem("Dantzig");
    ui->comboAlgo_2->addItem("Kruskal");
}

void MainWindow::MettreAJourExplications() {
    QString algo = ui->comboAlgo_2->currentText();
    QString res;

    if (algo == "Calcul des distances") {
        res = "Le calcul des distances consiste à créer une matrice des distances, "
              "où chaque case (i, j) contient la longueur minimale du chemin entre le sommet i et le sommet j, "
              "ou une valeur -1 s’il n’existe aucun chemin.\n\n"
              "➤ Ce que l'algorithme renvoie :\n"
              "Une matrice carrée contenant les distances minimales entre tous les couples de sommets.";
    }
    else if (algo == "Rangs des sommets") {
        res = "Le rang d’un sommet est la longueur du plus long chemin (en nombre d’arcs) partant d’un sommet sans prédécesseur (appelé source) jusqu’à ce sommet."
              " Cet algorithme peut etre utilisé uniquement sur des graphes orientés sans circuit.\n\n"
              "➤ Ce que l'algorithme renvoie :\n"
              "Un tableau contenant le rang de chaque sommet.";
    }
    else if (algo == "Tarjan") {
        res = "L’algorithme de Tarjan identifie les composantes fortement connexes dans un graphe orienté, c’est-à-dire des sous-ensembles "
              "de sommets tels que chaque sommet est accessible depuis n’importe quel autre du même ensemble.\n\n"
              "➤ Ce que l'algorithme renvoie :\n"
              "Une liste de composantes fortement connexes (chaque composante est un ensemble de sommets). L'algorithme crée"
              " également le graphe réduit lorsqu'il est éxécuté.";
    }
    else if (algo == "Points d'articulation et isthmes") {
        res = "Cet algorithme détecte les points d’articulation (sommets critiques) et les isthmes (arêtes critiques) dont la suppression "
              "entraîne une disconnexion du graphe.\n\n"
              "➤ Ce que l'algorithme renvoie :\n"
              "- Une liste des points d’articulation (sommets).\n"
              "- Une liste des isthmes (arêtes).";
    }
    else if (algo == "Dijkstra") {
        res = "L’algorithme de Dijkstra calcule les plus courts chemins depuis un sommet source vers tous les autres sommets dans un graphe "
              "valué avec des poids strictement positifs.\n\n"
              "➤ Ce que l'algorithme renvoie :\n"
              "- Un tableau de distances depuis la source.\n";
    }
    else if (algo == "Dantzig") {
        res = "L’algorithme de Dantzig (ou Floyd-Warshall) permet de calculer les plus courts chemins entre toutes les paires de sommets "
              "dans un graphe orienté pondéré, y compris avec des poids négatifs (à condition qu’il n’y ait pas de cycle négatif).\n\n"
              "➤ Ce que l'algorithme renvoie :\n"
              "Une matrice des plus courts chemins entre toutes les paires de sommets.";
    }
    else if (algo == "Kruskal") {
        res = "L’algorithme de Kruskal construit un arbre couvrant de poids minimal pour un graphe non orienté pondéré. "
              "Il choisit les arêtes les plus légères tout en évitant de former des cycles.\n\n"
              "➤ Ce que l'algorithme renvoie :\n"
              "- La liste des arêtes de l’arbre couvrant minimal.\n"
              "- Le poids total de cet arbre.";
    }
    else {
        res = "Sélectionnez un algorithme pour voir son explication.";
    }

    ui->TextEditExplicationsAlgos->setPlainText(res);
}




void MainWindow::on_actionExplications_triggered()
{
    remplirListeAlgorithmes2();
    ui->stackedWidget->setCurrentIndex(3); // adapte l'index
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
            res = "Le calcul des distances ne fonctionne que sur des graphes non valués.\n";
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
    else if (algo == "FS et APS") {
        res = "APS :\n";
        for (int i = 1; i < g->aps.size(); ++i)
            res += QString("%1 ").arg(g->aps[i]);
        res += "\n\nFS :\n";
        for (int i = 1; i < g->fs.size(); ++i)
            res += QString("%1 ").arg(g->fs[i]);
    }
    else if (algo == "Rangs des sommets") {
        if(!g->estOriente)
            res = "Algorithme impossible sur graphe non orienté";
        else{
            std::vector<int> rangs;
            if (Algorithms::calculRangs(*g, rangs)) {
                res = "Rangs des sommets :\n";
                for (int i = 1; i < rangs.size(); ++i)
                    res += QString("Sommet %1 : Rang %2\n").arg(i).arg(rangs[i]);
            } else {
                res = "Circuit détecté, pas de rangs.";
            }
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
            std::vector<int> pred;
            std::vector<int> dist = Algorithms::dijkstra(*gv, source, pred);

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


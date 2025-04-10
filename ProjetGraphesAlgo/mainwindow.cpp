#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ajoutergraphedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionAjouter_un_graphe, &QAction::triggered,
            this, &MainWindow::on_actionAjouter_un_graphe_triggered);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAjouter_un_graphe_triggered()
{
    AjouterGrapheDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString nom = dialog.getNom();
        bool oriente = dialog.estOriente();
        bool value = dialog.estValue();

        qDebug() << "✅ Nouveau graphe :" << nom << "| Valué :" << value << "| Orienté :" << oriente;
        // Tu pourras créer ton objet Graphe ici
    }
}

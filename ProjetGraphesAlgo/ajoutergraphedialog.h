// ajoutergraphedialog.h
#ifndef AJOUTERGRAPHEDIALOG_H
#define AJOUTERGRAPHEDIALOG_H

#include <QDialog>

namespace Ui {
class AjouterGrapheDialog;
}

class AjouterGrapheDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterGrapheDialog(QWidget *parent = nullptr);
    ~AjouterGrapheDialog();

    QString getNom() const;
    bool estValue() const;
    bool estOriente() const;

private:
    Ui::AjouterGrapheDialog *ui;
};

#endif // AJOUTERGRAPHEDIALOG_H

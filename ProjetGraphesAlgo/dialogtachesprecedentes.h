#ifndef DIALOGTACHESPRECEDENTES_H
#define DIALOGTACHESPRECEDENTES_H

#include "dialogordonnancement.h"
#include <QDialog>

namespace Ui {
class DialogTachesPrecedentes;
}

class DialogTachesPrecedentes : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTachesPrecedentes(const QList<Tache>& tachesExistantes, QWidget *parent = nullptr);
    ~DialogTachesPrecedentes();
    QList<int> getTachesPrecedentes() const;

private:
    Ui::DialogTachesPrecedentes *ui;
};

#endif // DIALOGTACHESPRECEDENTES_H

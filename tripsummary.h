#ifndef TRIPSUMMARY_H
#define TRIPSUMMARY_H

#include <QDialog>

namespace Ui {
class tripSummary;
}

class tripSummary : public QDialog
{
    Q_OBJECT

public:
    explicit tripSummary(QWidget *parent = nullptr);
    ~tripSummary();

private:
    Ui::tripSummary *ui;
};

#endif // TRIPSUMMARY_H

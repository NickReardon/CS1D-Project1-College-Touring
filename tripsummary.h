#ifndef TRIPSUMMARY_H
#define TRIPSUMMARY_H

#include <QDialog>
#include "dbmanager.h"
#include <qcheckbox.h>

namespace Ui {
class tripSummary;
}

class tripSummary : public QDialog
{
    Q_OBJECT

public:
    explicit tripSummary(QWidget *parent = nullptr);
    ~tripSummary();
    void displayTrips();

private:
    Ui::tripSummary *ui;
};

#endif // TRIPSUMMARY_H

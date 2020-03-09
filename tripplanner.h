#ifndef TRIPPLANNER_H
#define TRIPPLANNER_H

#include <QDialog>
#include "dbmanager.h"
#include <qcheckbox.h>
namespace Ui {
class tripPlanner;
}

class tripPlanner : public QDialog
{
    Q_OBJECT

public:
    explicit tripPlanner(QWidget *parent = nullptr);
    ~tripPlanner();


    void initializeList();
    void selectedCollegeList();
    void planAlgorithm(QString start);
    bool collegeDoesExist(QString colName);
    bool planDoesExist(QString colName);


private slots:
    void ChecboxChanged();
    void onPlanClick();

private:
    Ui::tripPlanner *ui;

    DbManager myDb = DbManager("C:/Users/Trevor Rizzi/Documents/College-Touring/college.db");
    QVector<QCheckBox*> checkBoxVector;
    QStringList selectedColleges; // a string to hold the selected colleges - should be helpful for the plan trip algorithm

    QStringList plannedColleges;



};

#endif // TRIPPLANNER_H

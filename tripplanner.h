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
    void updateCollegeTable();


private slots:
    void ChecboxChanged();
    void onPlanClick();

    void on_pushButton_2_clicked();

private:
    Ui::tripPlanner *ui;

    DbManager myDb = DbManager(PROJECT_PATH + "/college.db");

    QVector<QCheckBox*> checkBoxVector;
    QStringList selectedColleges; // a string to hold the selected colleges - should be helpful for the plan trip algorithm

    QStringList plannedColleges;//holds colleges in order for trip



};

#endif // TRIPPLANNER_H

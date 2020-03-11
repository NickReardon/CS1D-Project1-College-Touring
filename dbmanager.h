#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QAbstractItemModel>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QDebug"


static const QString PROJECT_PATH = "C:/Users/Trevor Rizzi/Documents/UP_CollegeProj/";


class DbManager
{


public:
    DbManager(const QString &path);

    void InitCollegeList(const QString &path);

    void initSouvenirList(const QString &path);

    void initDistanceList(const QString &path);

    void addUser(const QString &user, const QString &pass);
    void addSou(const QString &college, const QString &souName, const double &cost);
    void removeSou(const QString &souName, const QString &college);
    void updateSou(const QString &souName, const QString &college, const double &spin, const QString &newSouvenir);

    void addColleges(const QString &path);

    bool userExists(const QString &user);
    bool souExists(const QString &name, const QString &college);

    void clearDb();

    bool isOpen() const;

    bool checkAdmin(const QString &username) const;

    QString getPassword(const QString &username) const;

    void addTrip(QString tripID, QString plannedCollege, int index);

    bool tripIdExists(QString tripID);

    void createTripTable();


private:
    QSqlDatabase myDB;
};

#endif // DBMANAGER_H

#include "dbmanager.h"

DbManager::DbManager(const QString &path)
{
    myDB = QSqlDatabase::addDatabase("QSQLITE");

    myDB.setDatabaseName(path);

    if(!myDB.open())
    {
        qDebug() << "Error: can't connect to database!" << endl;

    }
    else
    {
        qDebug() << "Database: Connected" << endl;
    }

    QSqlQuery query;
    query.prepare("PRAGMA foreign_keys = ON");

    if(query.exec())
    {
        qDebug() << "foreign keys on" << endl;
    }


}

void DbManager::InitCollegeList(const QString &path)//path to the excel file
{
   QSqlDatabase fileDB = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
   fileDB.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + path);

   if(fileDB.open())
   {
       qDebug() << "Excel connection successful" << endl;

       QSqlQuery *query = new QSqlQuery(fileDB);
       query->exec("select * from [" + QString("Distances") + "$A1:A111]");   //Distances is the sheet name!

       QSqlQuery * querytoDb = new QSqlQuery(myDB);

       querytoDb->exec("CREATE TABLE Colleges ("
                       "collegeName TEXT);");

       QString compare = "";

       while(query->next())
       {

           if(myDB.open())
           {

               querytoDb->prepare("INSERT INTO Colleges(collegeName) values(:collegeName)");

               QString column1 = query->value(0).toString();

               if(compare != column1)
               {
                   querytoDb->bindValue(":collegeName",query->value(0).toString());
                   qDebug() << querytoDb->exec();

               }
               compare = column1;
           }
       }

       fileDB.close();
   }
}

void DbManager::initSouvenirList(const QString &path)
{
    QSqlDatabase fileDB = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
    fileDB.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + path);

    if(fileDB.open())
    {
        qDebug() << "Excel connection successful" << endl;

        QSqlQuery *query = new QSqlQuery(fileDB);
        query->exec("select * from [" + QString("Souvenirs") + "$A1:C61]");

        QSqlQuery * querytoDb = new QSqlQuery(myDB);

        querytoDb->exec("CREATE TABLE Souvenirs ("
                        "collegeName TEXT,"
                        "souvenirName TEXT,"
                        "cost DOUBLE);");

        QString tempColumn1;
        QString check;

        while(query->next())
        {
            if(myDB.open())
            {
                querytoDb->prepare("INSERT INTO Souvenirs(collegeName, souvenirName, cost) VALUES(:collegeName, :souvenirName, :cost)");

                QString column1 = query->value(0).toString();
                check = column1;
                if(column1 == "" || column1 == " ")
                {
                    column1 = tempColumn1;
                }

                QString column2 = query->value(1).toString();

                double column3 = query->value(2).toDouble();

                if(check == "" && column2 != "")
                {
                    querytoDb->bindValue(":collegeName", column1);
                    querytoDb->bindValue(":souvenirName",column2);
                    querytoDb->bindValue(":cost",column3);
                    qDebug() << querytoDb->exec();

                }

                tempColumn1 = column1;
            }
        }
        fileDB.close();
    }
}

void DbManager::initDistanceList(const QString &path)
{
    QSqlDatabase fileDB = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
    fileDB.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + path);

    if(fileDB.open())
    {
        qDebug() << "Excel connection successful" << endl;

        QSqlQuery *query = new QSqlQuery(fileDB);
        query->exec("select * from [" + QString("Distances") + "$A2:C111]");

        QSqlQuery * querytoDb = new QSqlQuery(myDB);

        querytoDb->exec("CREATE TABLE Distances ("
                        "startCollege TEXT,"
                        "endCollege TEXT,"
                        "distance INTEGER);");

        while(query->next())
        {
            //QSqlDatabase myDB = QSqlDatabase::database("sqlite_connection");

            if(myDB.open())
            {
                querytoDb->prepare("INSERT INTO Distances(startCollege, endCollege, distance) VALUES(:startCollege, :endCollege, :distance)");

                QString column1 = query->value(0).toString();

                QString column2 = query->value(1).toString();

                int column3 = query->value(2).toInt();

                querytoDb->bindValue(":startCollege", column1);
                querytoDb->bindValue(":endCollege",column2);
                querytoDb->bindValue(":distance",column3);
                qDebug() << querytoDb->exec();
            }
        }
        fileDB.close();
    }
}

//QSqlDatabase::removeDatabase("xlsx_connection"); // need to put this out of scope of the initialised db


void DbManager::addUser(const QString &user, const QString &pass)
{
    QSqlQuery *query = new QSqlQuery(myDB);
    QString userVal = "User";

   if(!userExists(user))
   {
    if(myDB.open())
    {
        query->exec("CREATE TABLE Logins ("
                    "Username TEXT,"
                    "Password TEXT,"
                    "UserValue TEXT);");

        query->prepare("INSERT INTO Logins(Username, Password, UserValue) VALUES(:Username, :Password, :UserValue)");

        query->bindValue(":Username", user);
        query->bindValue(":Password", pass);
        query->bindValue(":UserValue", userVal);
        qDebug() << query->exec();
    }
   }

}

bool DbManager::userExists(const QString &user)
{
    bool exists = false;

    QSqlQuery *checkQuery = new QSqlQuery(myDB);

    checkQuery->prepare("SELECT Username FROM Logins WHERE (Username) = (:Username)");
    checkQuery->bindValue(":Username", user);

    if(checkQuery->exec())
    {
        if(checkQuery->next())
        {
                exists = true;
                QString userName = checkQuery->value("Username").toString();
                qDebug() << userName;
        }
    }
    else
    {
        qDebug() << "person exists failed: " << checkQuery->lastError();
    }

    return exists;
}

void DbManager::clearDb()
{
    QSqlQuery *deleteQuery = new QSqlQuery(myDB);

    deleteQuery->prepare("DROP TABLE IF EXISTS Colleges");

    deleteQuery->exec();

    deleteQuery->prepare("DROP TABLE IF EXISTS Distances");

    deleteQuery->exec();

    deleteQuery->prepare("DROP TABLE IF EXISTS Souvenirs");

    deleteQuery->exec();

}

bool DbManager::isOpen() const
{
    return myDB.isOpen();
}

bool DbManager::checkAdmin(const QString &username) const
{
    bool admin = false;

    QSqlQuery *checkQuery = new QSqlQuery(myDB);

    checkQuery->prepare("SELECT UserValue FROM logins where (Username) = (:Username)");
    checkQuery->bindValue(":Username", username);

    if(checkQuery->exec())
    {
        if(checkQuery->next())
        {
            QString userVal = checkQuery->value("UserValue").toString();

            if(userVal == "admin")
            {
                admin = true;
            }
            qDebug() << admin;
        }
    }
    else
    {
        qDebug() << "Admin exists failed:";

    }

    if(admin)
    {
        qDebug() << "Admin";
    }

    return admin;
}

QString DbManager::getPassword(const QString &username) const
{
    QString password;
    QSqlQuery *getQuery = new QSqlQuery(myDB);
        getQuery->prepare("SELECT Password FROM logins WHERE (Username) = (:Username)");
        getQuery->bindValue(":Username", username);

        if (getQuery->exec())
        {
            if (getQuery->next())
            {
                password = getQuery->value("Password").toString();
                qDebug() << password;
            }
            else
            {
                qDebug() << "next query fail: " << getQuery->lastError();
            }
        }
        else
        {
            qDebug() << "person 1exists failed: " << getQuery->lastError();
        }

        return password;
}

void DbManager::addColleges(const QString &path)
{
    QSqlDatabase fileDB = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
    fileDB.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + path);

    if(fileDB.open())
    {
        qDebug() << "Excel connection successful" << endl;

        QSqlQuery *query = new QSqlQuery(fileDB);
        query->exec("select * from [" + QString("New Campuses") + "$A2:C47]");

        QSqlQuery * querytoDb = new QSqlQuery(myDB);
        QSqlQuery * querytoList = new QSqlQuery(myDB);

        while(query->next())
        {
            if(myDB.open())
            {
                querytoDb->prepare("INSERT INTO Distances(startCollege, endCollege, distance) VALUES(:startCollege, :endCollege, :distance)");
                querytoList->prepare("INSERT OR REPLACE INTO Colleges(collegeName) values(:collegeName)");

                QString column1 = query->value(0).toString();
                QString column2 = query->value(1).toString();

                int column3 = query->value(2).toInt();

                querytoDb->bindValue(":startCollege", column1);
                querytoDb->bindValue(":endCollege",column2);
                querytoDb->bindValue(":distance",column3);
                qDebug() << querytoDb->exec();


                {
                    querytoList->bindValue(":collegeName",query->value(0).toString());
                    qDebug() << querytoList->exec();

                }

            }
        }

        fileDB.close();
    }
}

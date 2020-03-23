#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QAbstractItemModel>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QDebug"

/**
 * @file
 */    
    
/**
 * \class DbManager
 *
 * \brief SQL Database Manager class
 *
 * DbManager sets up the connection with SQL database
 * and performs some basics queries. The class requires
 * existing SQL database. You can create it with sqlite:
 * 1. $ sqlite3 people.db
 * 2. sqilte> CREATE TABLE people(ids integer primary key, name text);
 * 3. sqlite> .quit
 */

    
static const QString PROJECT_PATH = "C:/Users/farna/Documents/CS1D-Project1-College-Touring/";


class DbManager
{


public:
     /**
     * @brief Constructor
     *
     * Constructor sets up connection with db and opens it
     * @param path - absolute path to db file
     */
    DbManager(const QString &path);

     /**
     * @brief initalizes sql data table for college and inputs all data from path parameter file location
     * @param &path - filepath for DB
     * @return void - none
     */
    void InitCollegeList(const QString &path);

     /**
     * @brief initalizes sql data table for souvenir and inputs all data from path parameter file location
     * @param &path - filepath for DB
     * @return void - none
     */
    void initSouvenirList(const QString &path);
     /**
     * @brief initalizes sql data table for college distances and inputs all data from path parameter file location
     * @param &path - filepath for DB
     * @return void - none
     */
    void initDistanceList(const QString &path);

     /**
     * @brief adds a user to DB for future logins
     * @param &user - user name
     * @param &pass - password
     * @return void - none
     */
    void addUser(const QString &user, const QString &pass);
     /**
     * @brief adds a souv to DB for data tables
     * @param &college - college containing souv
     * @param &souName - name of souv
     * @param &cost - cost of souv
     * @return void - none
     */
    void addSou(const QString &college, const QString &souName, const double &cost);
     /**
     * @brief removes a souv to DB for data tables
     * @param &college - college containing souv
     * @param &souName - name of souv
     * @return void - none
     */
    void removeSou(const QString &souName, const QString &college);
     /**
     * @brief updates souv to DB for data tables with new values
     * @param &college - college containing souv
     * @param &souName - name of souv
     * @param &cost - cost of souv
     * @param &newSovenir - new object data
     * @return void - none
     */
    void updateSou(const QString &souName, const QString &college, const double &spin, const QString &newSouvenir);

     /**
     * @brief adds souv to cart
     * @param count - quantity of items
     * @param souvenir - name of souv
     * @param price - cost of souv
     * @return void - none
     */
    void addCart(const QString trip, const QString college, const QString souvenir, const double price, const int count);
     /**
     * @brief updates total items in carts with accurate corresponding data
     * @param count - quantity of items
     * @param souvenir - name of souv
     * @param college - name of college selling souv
     * @return void - none
     */
    void updateCart(const QString college, const QString souvenir, const int count);
     /**
     * @brief returns quanitity of items in cart
     * @param souvenir - name of souv
     * @param college - name of college selling souv
     * @return int - quanity of cart
     */
    int cartQuantity(const QString college, const QString souvenir);

     /**
     * @brief delete item from cart
     * @param souvenir - name of souv
     * @param college - name of college selling souv
     * @return void - none
     */
    void removeCart(const QString college, const QString souvenir);

     /**
     * @brief adds purchase to DB and cart
     * @return void - none
     */
    void addPurchase();
     /**
     * @brief clear all items from cart
     * @return void - none
     */
    void resetCart();

     /**
     * @brief add colleges from file to DB
     * @param path - absolute path to db file souv
     * @return void - none
     */
    void addColleges(const QString &path);

     /**
     * @brief checks to see if the username exists
     * @param name of the user
     * @return true - username exists, false - username does not exist
     */
    bool userExists(const QString &user);
     /**
     * @brief checks to see if the souv exists
     * @param &name - name of souv
     * @param &college - college that contains souv
     * @return true - username exists, false - username does not exist
     */
    bool souExists(const QString &name, const QString &college);

     /**
     * @brief clears database of all data
     * @return void - nothing
     */
    void clearDb();
    /**
     * @brief checks to see if DbManager is open
     * @return true - DbManager is open, false - DbManager is closed
     */
    bool isOpen() const;
    /**
     * @brief checks to see if the username entered is an admin
     * @param username of the user
     * @return true - will enable admin login, false - the person is not an admin
     */
    bool checkAdmin(const QString &username) const;
    /**
     * @brief Gets password from user
     * @param name of the user(s)
     * @return obtains the password from user
     */
    QString getPassword(const QString &username) const;

     /**
     * @brief adds a trip to appropiate dataTables
     * @param tripID - passed in trip ID
     * @param plannedCollege - holds selected college name
     * @param index- position for data structure
     * @return void - nothing
     */
    void addTrip(QString tripID, QString plannedCollege, int index);

     /**
     * @brief returns whether tripID data exists in DB
     * @param tripID- trip id to verify
     * @return bool- whether data exists
     */
    bool tripIdExists(QString tripID);

    void createTripTable();


private:
        /**
     * @brief decalre database field memberobj
     */
    QSqlDatabase myDB;
};

#endif // DBMANAGER_H

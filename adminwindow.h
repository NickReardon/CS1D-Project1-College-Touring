#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include <QDialog>
#include "addsouvenir.h"

/**
 * @file
 */


/**
 * \class AdminWindow
 *
 * \brief class initializes AdminWindow UI and manages all Admin only permissive execution in editing user database, college database, souv database, etc
 *
 * AdminWindow allows admin users only to intialize lists, edit lists, add individual college, souv data values
 * 
 */
    
namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
     /**
     * @brief explicit default Constructor
     *
     * Constructor intializes UI for adminwindow
     * calls updateCollegeTable() and updateSouvenirTable()
     * @param *parent - for QDialog
     */
    explicit AdminWindow(QWidget *parent = nullptr);
     /**
     * @brief updates sql data table for colleges when called
     * @return void - none
     */
    void updateCollegeTable();
     /**
     * @brief updates sql data table for college souvenirs when called
     * @return void - none
     */
    void updateSouvenirTable();
    
    /**
    * @brief Destructor
    *
    * Close the AdminWindow UI
    */
    ~AdminWindow();

private slots:
    
    /**
    * @brief runs a verification of confirmPage, if verfied, initalizes distance, college, souvenir lists, updates appropiate data tables and UI models
    * @return void - none
    */
    void onInitialise();
    /**
    * @brief clears database of data
    * @return void - none
    */
    void clearCollegeData();
    /**
    * @brief executes addSouvenir class obj
    * @return void - none
    */
    void on_addSouvenir_released();

    /**
    * @brief deletes souvenir from data tables
    * @return void - none
    */
    void on_removeSouvenir_released();
    /**
    * @brief adds college to connected database
    * @return void - none
    */
    void onAddColleges();

    /**
    * @brief displays and prints data from souvenir data set to UI
    * @return void - none
    */
    void on_souvenirView_clicked(const QModelIndex &index);

    void on_removeSouvenir_2_clicked();

private:
        /**
    * @brief string stores temp souv data
    */
    QString souve;
     /**
     * @brief points to adminwindow UI
     */
    Ui::AdminWindow *ui;

     /**
     * @brief assigns DB to predefined PROJECT_PATH constant
     */
    DbManager myDb = DbManager(PROJECT_PATH + "/college.db");

};

#endif // ADMINWINDOW_H

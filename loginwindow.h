#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include "adminwindow.h"
#include "tripplanner.h"
QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

/**
 * @file
 */


/**
 * \class loginWindow
 *
 * \brief class sets ui and manages execution for validating username and password
 *
 * loginWindow validates the credentials entered by the user, conncects to the database, and prompts the appropiate screen for the appropate user to execute after the user has been logged in and validated
 * 
 */

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public slots:
     /**
     * @brief executes tripPlanner class obj
     * @return none- void
     */
    void onPlanClick();
     /**
     * @brief executes login sequence for user
     * validates user and pass and usertype
     * @return none- void
     */
    void onLoginClick();
     /**
     * @brief executes createuser
     * @return none- void
     */
    void onCreateClick();
         /**
     * @brief clears dialog
     * @return none- void
     */
    void onClearClick();


public:
    /**
    * @brief default explicit Constructor
    *
    * Constructor sets up ui for loginwindow
    * creates a tripTable and verifies connection
    * to database
    * @param *parent - for QDialog
    */
    LoginWindow(QWidget *parent = nullptr);
    /**
    * @brief Destructor
    *
    * Close the loginwindow UI
    */
    ~LoginWindow();

private slots:
    /**
    * @brief execute onLoginClock
    * @return none-void
    */
    void on_Username_returnPressed();

    /**
    * @brief execute onLoginClock
    * @return none-void
    */
    void on_Password_returnPressed();
    /**
    * @brief execute onLoginClock
    * @return none-void
    */
    void on_loginButton_pressed();

private:
     /**
     * @brief points to loginwindow UI
     */
    Ui::LoginWindow *ui;

     /**
     * @brief assigns DB to predefined PROJECT_PATH constant
     */
    DbManager myDb = DbManager(PROJECT_PATH + "/college.db");

     /**
     * @brief pointer for instance of adminwindow for adminType
     */
    AdminWindow *admin;

};
#endif // LOGINWINDOW_H

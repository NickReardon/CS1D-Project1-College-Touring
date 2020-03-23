#ifndef ADDSOUVENIR_H
#define ADDSOUVENIR_H

#include <QDialog>
#include "dbmanager.h"
#include "confirmpage.h"

namespace Ui {
class addSouvenir;
}

class addSouvenir : public QDialog
{
    Q_OBJECT

public:
     /**
     * @brief explicit default Constructor
     *
     * Constructor intializes UI for addSouvenir
     * calls updateCombo()
     * @param *parent - for QDialog
     */
    explicit addSouvenir(QWidget *parent = nullptr);
    /**
    * @brief Destructor
    *
    * Close the addSouvenir UI
    */
    ~addSouvenir();

    /**
    * @brief Destructor
    *
    * updates sql query for startcollege and sets ui Model
    */
    void updateCombo();

private slots:
    void on_pushButton_2_released();
    /**
    * @brief executes process for adding a souvenir to database
    *
    * Close the loginwindow UI
    */
    void on_pushButton_released();


private:
     /**
     * @brief points to addSouvenir UI
     */
    Ui::addSouvenir *ui;
     /**
     * @brief assigns DB to predefined PROJECT_PATH constant
     */
    DbManager myDb = DbManager(PROJECT_PATH + "/college.db");


};

#endif // ADDSOUVENIR_H

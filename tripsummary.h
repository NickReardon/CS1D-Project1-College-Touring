#ifndef TRIPSUMMARY_H
#define TRIPSUMMARY_H

#include <QDialog>
#include "dbmanager.h"

/**
 * @file
 */


/**
 * \class tripSummary
 *
 * \brief class for generating and updating sql data from read DB and overall planned trip summary
 *
 * tripSummary displays an overview of what the user has planned for his trip
 * this updates all data from souvenier, collegename, price, quantity, etc for the plan tripped
 * 
 */

namespace Ui {
class tripSummary;
}

class tripSummary : public QDialog
{
    Q_OBJECT

public:
    /**
    * @brief default explicit Constructor
    *
    * Constructor sets up ui for tripSummary
    * @param *parent - for QDialog
    */
    explicit tripSummary(QWidget *parent = nullptr);
    
    /**
    * @brief explicit Constructor
    *
    * Constructor sets up ui for tripSummary
    and initilalizes tripId
    * @param *parent - for QDialog
    * @param id - tripID
    */
    explicit tripSummary(const QString& id, QWidget *parent = nullptr);

    /**
    * @brief Destructor
    *
    * Close the tripSummary UI
    */
    ~tripSummary();



private slots:
    
     /**
     * @brief prepares a query and binds tripId and collegeNames to appropiate Database
     * @return nothing - void
     */
    void on_school_comboBox_currentIndexChanged(const QString &arg1);

     /**
     * @brief updates tripID and UI model when called. Prepares query for all availble tripIDs
     * @return nothing - void
     */
    void updateTripSchoolList();

private:
     /**
     * @brief points to tripSummary UI
     */
    Ui::tripSummary *ui;
     /**
     * @brief assigns DB to predefined PROJECT_PATH constant
     */
    DbManager myDb = DbManager(PROJECT_PATH + "/college.db");

    QString tripID;
};

#endif // TRIPSUMMARY_H

#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QDialog>
#include "QDebug"
    
/**
 * @file
 */


/**
 * \class confirmPage
 *
 * \brief class sets breifs user on confirmation on trip planning actions
 *
 * 
 */

namespace Ui {
class confirmpage;
}

class confirmpage : public QDialog
{
    Q_OBJECT

public:
     /**
     * @brief explicit default Constructor
     *
     * Constructor intializes UI for confirmpage
     * @param *parent - for QDialog
     */
    explicit confirmpage(QWidget *parent = nullptr);
    
    /**
    * @brief Destructor
    *
    * Close the loginwindow UI
    */
    ~confirmpage();

    confirmpage(QWidget *parent, bool &returnAnswer);
    
    /**
    * @brief returns answer to calling function
    * @return bool- answer
    */
    bool getData();

private slots:
    /**
    * @brief sets answer to true, hides ui
    * @return void- none
    */
    void on_okButton_released();
    /**
    * @brief sets answer to false, hides ui
    * @return void- none
    */
    void on_cancelButton_released();

private:
    
     /**
     * @brief points to confirmpage UI
     */
    Ui::confirmpage *ui;
    
     /**
     * @brief declares boolean to default val
     */
    bool answer = false;
};

#endif // CONFIRMPAGE_H

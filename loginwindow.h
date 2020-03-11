#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include "adminwindow.h"
#include "tripplanner.h"
QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void onPlanClick();
    void onLoginClick();
    void onCreateClick();
    void onClearClick();


public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_Username_returnPressed();

    void on_Password_returnPressed();

    void on_loginButton_pressed();

private:
    Ui::LoginWindow *ui;


    DbManager myDb = DbManager(PROJECT_PATH + "/college.db");

    AdminWindow *admin;

};
#endif // LOGINWINDOW_H

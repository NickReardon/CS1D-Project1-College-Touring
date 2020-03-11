#ifndef TRIPPROGRESS_H
#define TRIPPROGRESS_H

#include <QDialog>

namespace Ui {
class tripProgress;
}

class tripProgress : public QDialog
{
    Q_OBJECT

public:
    explicit tripProgress(QWidget *parent = nullptr);
    ~tripProgress();

private:
    Ui::tripProgress *ui;
};

#endif // TRIPPROGRESS_H

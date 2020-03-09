#include "tripplanner.h"
#include "ui_tripplanner.h"

#include "QVBoxLayout"

tripPlanner::tripPlanner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tripPlanner)
{
    ui->setupUi(this);

    initializeList();
    updateCollegeTable();
    updateSouvTable();
}

tripPlanner::~tripPlanner()
{
    delete ui;
}


void tripPlanner::initializeList()
{
    QSqlQuery *query = new QSqlQuery();

    checkBoxVector.clear();

    QWidget *container = new QWidget;
    QVBoxLayout *vBoxLayout = new QVBoxLayout;

    container->setLayout(vBoxLayout);

    ui->CollegeList_scrollArea->setWidget(container);



    query->prepare("SELECT DISTINCT startCollege FROM Distances");
    if(!query->exec())
    {
        qDebug() << "tripPlanner initializeList query failed";


    }
    else
    {
        while(query->next())
        {
            QCheckBox* checkBox = new QCheckBox(query->value(0).toString());
            checkBox->setCheckState(Qt::CheckState::Unchecked);
            checkBoxVector.push_back(checkBox);
            connect(checkBox, &QCheckBox::stateChanged, this, &tripPlanner::ChecboxChanged);
        }

        for(int i = 0; i < checkBoxVector.size(); i++)
        {
            vBoxLayout->addWidget(checkBoxVector[i]);
        }
    }
}

void tripPlanner::ChecboxChanged()
{

    qDebug() << "Signal caught";

    int checkedCount = 0;

    for(int i = 0; i < checkBoxVector.size(); i++)
    {
        if(checkBoxVector[i]->checkState() == Qt::CheckState::Checked)
        {
            checkedCount++;
        }
    }

    if(checkedCount == 11)
    {
        for(int i = 0; i < checkBoxVector.size(); i++)
        {
            if(checkBoxVector[i]->checkState() == Qt::CheckState::Unchecked)
            {
                checkBoxVector[i]->setDisabled(true);
            }
            qDebug() << checkBoxVector[i] << endl;
        }
    }
    else
    {
        for(int i = 0; i < checkBoxVector.size(); i++)
        {
            checkBoxVector[i]->setDisabled(false);
        }
    }
}

void tripPlanner::selectedCollegeList()// creates a list of the selected colleges from the checked boxes
{
    selectedColleges.clear();
    QSqlQuery *query = new QSqlQuery();
    int i = 0;

    query->prepare("SELECT DISTINCT startCollege FROM Distances");
    if(!query->exec())
    {
        qDebug() << "tripPlanner initializeList query failed";


    }
    else
    {
        while(query->next())
        {
            if(checkBoxVector[i]->checkState() == Qt::CheckState::Checked)
            {
                QString temp = query->value("startCollege").toString();
                selectedColleges << temp;
                qDebug()<< temp << endl;

            }
            i++;
        }

    }

}

void tripPlanner::onPlanClick()
{
    QString startingCollege;
    startingCollege = this->ui->colName->text();

    selectedCollegeList();

    if(!collegeDoesExist(startingCollege)) // checks if the user input a correct college
    {
        this->ui->warningLabel->setText("Please enter a college that you have selected!");
    }
    else
    {
        this->ui->warningLabel->setText("");
        planAlgorithm(startingCollege); // will plan the trip

    }

}

void tripPlanner::planAlgorithm(QString start)// start is the user selected starting college
{
    QSqlQuery *query = new QSqlQuery;

    query->prepare("SELECT distance, endCollege FROM Distances WHERE startCollege = (:start) ORDER BY distance ASC");
    query->bindValue(":start", start);

    if(query->exec())
    {
        while(query->next())// goes to next college
        {

            QString temp = query->value("endCollege").toString();
            if(!planDoesExist(temp) && collegeDoesExist(temp))
            {
                plannedColleges<< start;
                start = query->value("endCollege").toString();
                planAlgorithm(start); // recursive call
            }

        }
    }
    else
    {
        qDebug() << "failed";
    }

}

bool tripPlanner::collegeDoesExist(QString colName)// checks if a college is added to the trip
{
    for(int i = 0; i < selectedColleges.size(); i++)
    {
        if(colName == selectedColleges[i])
        {
            return true;
        }
    }

    return false;
}

bool tripPlanner::planDoesExist(QString colName) // checks if a college is in the planned list
{
    for(int i = 0; i < plannedColleges.size(); i++)
    {
        if(colName == plannedColleges[i])
        {
            return true;
        }
    }

    return false;
}

void tripPlanner::updateCollegeTable()
{
    QSqlQueryModel* model=new QSqlQueryModel();

    QSqlQuery* qry=new QSqlQuery();

    qry->prepare("SELECT distance, endCollege FROM Distances WHERE startCollege = 'Saddleback College'");

    if(qry->exec())
    {
        qDebug() << "college table updated.";
    }

    model->setQuery(*qry);

    ui->sadView->setModel(model);
    ui->sadView->setColumnWidth(20, 250);
}

void tripPlanner::updateSouvTable()
{
    QSqlQueryModel* model=new QSqlQueryModel();

    QSqlQuery* qry=new QSqlQuery();

    qry->prepare("SELECT souvenirName, collegeName FROM Souvenirs");

    if(qry->exec())
    {
        qDebug() << "college table updated.";
    }

    model->setQuery(*qry);

    ui->sadView_2->setModel(model);
    ui->sadView_2->setColumnWidth(20, 400);
}

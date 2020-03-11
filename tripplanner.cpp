#include "tripplanner.h"
#include "ui_tripplanner.h"

#include "QVBoxLayout"

tripPlanner::tripPlanner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tripPlanner)
{
    ui->setupUi(this);

    initializeList();
    updateCombo();
}

tripPlanner::~tripPlanner()
{
    delete ui;
}

void tripPlanner::updateCombo()
{
    QSqlQueryModel* model=new QSqlQueryModel();

    QSqlQuery* qry=new QSqlQuery();

    qry->prepare("SELECT DISTINCT startCollege FROM Distances");

    if(qry->exec())
    {
        qDebug() << "college1 table updated.";
    }
    else
        qDebug() << "failed";

    model->setQuery(*qry);

    ui->colName->setModel(model);
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
    QString tripID;
    startingCollege = this->ui->colName->currentText();
    tripID = this->ui->trip->text();

    selectedCollegeList();

    if(!collegeDoesExist(startingCollege)) // checks if the user input a correct college
    {
        this->ui->warningLabel->setText("Please enter a college that you have selected on the left!");
    }
    else
    {
        updateSouvTable(startingCollege);
        updateCollegeTable(startingCollege);

        qDebug() << myDb.tripIdExists(tripID);
        if(tripID.size() == 3 && !myDb.tripIdExists(tripID))
        {
            this->ui->warningLabel->setText("");
            this->ui->tripWarning->setText("");
            planAlgorithm(startingCollege); // will plan the trip
            for(int index = 0; index < plannedColleges.size(); index++)
            {
                myDb.addTrip(tripID, plannedColleges[index], index); // uploads trip to DB
            }
            showTrip(tripID);
            //tripSummary *summary = new tripSummary(this);
            //summary->show();
        }
        else
        {
            this->ui->tripWarning->setText("That trip ID is taken or invalid!");
        }

    }

    for(int i = 0; i < plannedColleges.size(); i++)
                {
                    qDebug() << plannedColleges[i];
                }

}

void tripPlanner::planAlgorithm(QString start)// start is the user selected starting college
{
    plannedColleges<< start;
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

void tripPlanner::updateCollegeTable(QString start)
{
    this->ui->collegeLabel->setText("College distances from: " + start);
    QSqlQueryModel* model=new QSqlQueryModel();

    QSqlQuery* qry=new QSqlQuery();

    qry->prepare("SELECT endCollege, distance FROM Distances WHERE startCollege = (:startCollege)");
    qry->bindValue(":startCollege", start);

    if(qry->exec())
    {
        qDebug() << "college table updated.";
    }

    model->setQuery(*qry);

    ui->sadView->setModel(model);
    ui->sadView->setColumnWidth(20, 250);
}

void tripPlanner::updateSouvTable(QString start)
{
    this->ui->souvLabel->setText("Souvenirs at: " + start);
    QSqlQueryModel* model=new QSqlQueryModel();

    QSqlQuery* qry=new QSqlQuery();

    qry->prepare("SELECT souvenirName FROM Souvenirs WHERE collegeName = (:startCollege)");
    qry->bindValue(":startCollege", start);

    if(qry->exec())
    {
        qDebug() << "college table updated.";
    }
    else
        qDebug() << "failed";

    model->setQuery(*qry);

    ui->sadView_2->setModel(model);
    ui->sadView_2->setColumnWidth(20, 400);

}

void tripPlanner::showTrip(QString ID)
{
    QSqlQueryModel* model=new QSqlQueryModel();

    QSqlQuery* qry=new QSqlQuery();
    qDebug() << ID;

    qry->prepare("SELECT college FROM Trips WHERE tripID = (:tripId)");
    qry->bindValue(":tripId" , ID);
    if(qry->exec())
    {
        qDebug() << "trip table updated.";
    }
    else
        qDebug() << "failed trip table";

    model->setQuery(*qry);

    ui->window->setModel(model);
    ui->window->setColumnWidth(20, 400);
}






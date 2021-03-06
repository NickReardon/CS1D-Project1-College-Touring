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
    distance = 0;
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
            plannedColleges.clear();
            planAlgorithm(startingCollege, distance); // will plan the trip
            distanceTo << 0; // adds 0 for distance to next college at last college
            for(int index = 0; index < plannedColleges.size(); index++)
            {
                myDb.addTrip(tripID, plannedColleges[index], index, distanceTo[index]); // uploads trip to DB
            }
            showTrip(tripID);
            id = tripID;

            QString out = QString::number(distance);
            this->ui->dist->setText(out);

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

void tripPlanner::planAlgorithm(QString start, int dist)// start is the user selected starting college
{
    int distTo;
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
                dist+= query->value("distance").toInt();
                distTo = query->value("distance").toInt();
                distanceTo << distTo;
                qDebug() << "--------------------";
                qDebug() << distanceTo[0];
                qDebug() << distTo;
                distance = dist;
                start = query->value("endCollege").toString();
                planAlgorithm(start, dist); // recursive call

            }

        }
    }
    else
    {
        qDebug() << "failed";
    }

    qDebug() << distance;

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
    ui->sadView->resizeColumnsToContents();
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
    ui->sadView_2->resizeColumnsToContents();

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

    //clear here?
    ui->window->setModel(model);
    ui->window->resizeColumnsToContents();
}

void tripPlanner::onDisplayClick()
{
    QString startingCollege;
    startingCollege = this->ui->colName->currentText();

    updateSouvTable(startingCollege);
    updateCollegeTable(startingCollege);


}

void tripPlanner::onStartClick()
{

    if(!plannedColleges.isEmpty())
    {
        this->ui->startLabel->setText("");
        this->hide();
        tripprogress *prog = new tripprogress(this);

        prog->tripID = id;
        qDebug() << prog->tripID;
        prog->show();
    }
    else
    {
        this->ui->startLabel->setText("Please create a trip first!");
    }


}







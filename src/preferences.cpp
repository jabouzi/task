#include "preferences.h"
//
Preferences::Preferences( QWidget * parent, Qt::WFlags f) 
    : QDialog(parent, f)
{
    setupUi(this);   
    connect(this->saveButton, SIGNAL(clicked()), this, SLOT(save()));    
    connect(removeButton, SIGNAL(clicked()), this, SLOT(deleteProject()));
}

void Preferences::initPath(QString lpath)
{
    path = lpath;
    initDB();
    setRepeatTime();
    setHistoryDays();    
    setProjectsList();
}

void Preferences::initDB()
{
    db = Database::getInstance();
    db->setDatabaseName(path+"data/.task.db");
    db->setDatabase();   
}

void Preferences::setRepeatTime()
{  
    db->setTable("prefTable");
    db->where("id = 1");
    int timer = db->select("timer").toInt();     
    minutesBox->setValue(timer);
    db->where("");
}

void Preferences::setHistoryDays()
{
    db->setTable("prefTable");
    db->where("id = 1");
    int htime = db->select("history").toInt();       
    daysBox->setValue(htime);
    db->where("");
}

void Preferences::setProjectsList()
{
    projectsList->clear();
    QStringList titlesList;    
    db->setTable("tasksTable");
    titlesList = db->selectAllDistinct("title");
    titlesList.sort();  
    projectsList->addItems(titlesList);
}

void Preferences::deleteProject()
{
    QString sql = "DELETE FROM tasksTable WHERE title = '"+projectsList->currentText()+"'";
    db->sqlQuery(sql);
    setProjectsList();
    emit(preferencesChanged());
}

void Preferences::save()
{
    db->setTable("prefTable");
    db->where("id = 1");
    db->update("timer", QString::number(minutesBox->value()));
    db->update("history", QString::number(daysBox->value()));
    db->where("");
    emit(preferencesChanged());
}

void Preferences::updateList()
{
    setProjectsList();
}
//

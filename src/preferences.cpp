#include "preferences.h"
//
Preferences::Preferences( QWidget * parent, Qt::WFlags f) 
    : QDialog(parent, f)
{
    setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/.task.db");
    setRepeatTime();
    setHistoryDays();    
    setProjectsList();
    connect(this->saveButton, SIGNAL(clicked()), this, SLOT(save()));    
    connect(removeButton, SIGNAL(clicked()), this, SLOT(deleteProject()));
}

void Preferences::setRepeatTime()
{
    db.open();    
    QSqlQuery query;
    query.exec("SELECT timer FROM prefTable WHERE id = 1");
    int field = query.record().indexOf("timer");
    query.next();
    minutesBox->setValue(query.value(field).toInt());
}

void Preferences::setHistoryDays()
{
    db.open();    
    QSqlQuery query;
    query.exec("SELECT history FROM prefTable WHERE id = 1");
    int field = query.record().indexOf("history");
    query.next();
    daysBox->setValue(query.value(field).toInt());
}

void Preferences::setProjectsList()
{
    projectsList->clear();
    QStringList titlesList;
    db.open();    
    QSqlQuery query;
    query.exec("SELECT DISTINCT title FROM tasksTable");
    int field = query.record().indexOf("title");
    while (query.next()) {
        titlesList << query.value(field).toString();
    }
    titlesList.sort();
    projectsList->addItems(titlesList);
}

void Preferences::deleteProject()
{
    db.open();    
    QSqlQuery query;
    QString sql = "DELETE FROM tasksTable WHERE title = '"+projectsList->currentText()+"'";
    query.exec(sql);
    setProjectsList();
    emit(preferencesChanged());
}

void Preferences::save()
{
    db.open();    
    QSqlQuery query;
    query.exec("UPDATE prefTable set timer = "+QString::number(minutesBox->value())+" WHERE id = 1");
    query.exec("UPDATE prefTable set history = "+QString::number(daysBox->value())+" WHERE id = 1");
    emit(preferencesChanged());
}

void Preferences::updateList()
{
    setProjectsList();
}
//

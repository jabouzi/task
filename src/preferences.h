#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QtSql>
#include "ui_preferences.h"
#include "database.h"

class Preferences : public QDialog, public Ui::Preferences
{
Q_OBJECT
public:
    Preferences( QWidget * parent = 0, Qt::WFlags f = 0 );
    void initPath(QString);
    void updateList();    
    
private:
    void initDB();
    void setRepeatTime();
    void setHistoryDays();
    void setProjectsList();   
    QString path; 
    Database *db;
    
private slots:
    void deleteProject();
    void save();    
    
signals:
    void preferencesChanged();
    
};
#endif

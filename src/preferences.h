#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QtSql>
#include "ui_preferences.h"

class Preferences : public QDialog, public Ui::Preferences
{
Q_OBJECT
public:
    Preferences( QWidget * parent = 0, Qt::WFlags f = 0 );
    void updateList();
    QSqlDatabase db;
    
private:
    void setRepeatTime();
    void setHistoryDays();
    void setProjectsList();    
    
private slots:
    void deleteProject();
    void save();    
    
signals:
    void preferencesChanged();
    
};
#endif

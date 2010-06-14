#ifndef TASKMANAGER_H
#define TASKMANAGER_H
//
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include "ui_taskManager.h"
#include "preferences.h"
//
class TaskManager : public QMainWindow, public Ui::TaskManager
{
Q_OBJECT
public:
	TaskManager( QWidget * parent = 0, Qt::WFlags f = 0 );
    
private:
    void adjustWindow();    
    void setTimes();
    void createTryIcon();
    void createActions();
    void createTable(QString);
    bool tableExists(QString);
    Preferences prefs;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *prefsAction;
    QAction *quitAction;    
    QAction *restoreAction;    
    QTimer * timer;
    QString lastTitle;
    
private slots:
    void save();
    void startTimer();
    void showManager();
    void showPreferences();
    void setProjectsList();
    void about();
    void showTasks();
    
signals:
    void itsTime();

};
#endif





#include "taskManager.h"
//
TaskManager::TaskManager( QWidget * parent, Qt::WFlags f) 
    : QMainWindow(parent, f)
{
    setupUi(this);    
    if (!tableExists("tasksTable"))
    {
        createTable("tasksTable");
    }
    if (!tableExists("prefTable"))
    {
        createTable("prefTable");
    }
    adjustWindow();
    createActions();
    createTryIcon();
    setWindowTitle(tr("Task reminder"));
    QStringList label;
    label << tr("Task detail");
    taskTable->setHorizontalHeaderLabels(label);
    setProjectsList();
    setTimes();
    timer = new QTimer(this);
    startTimer();
    connect(&prefs, SIGNAL(preferencesChanged()), this, SLOT(startTimer()));
    connect(&prefs, SIGNAL(preferencesChanged()), this, SLOT(setProjectsList()));
    connect(this->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(timer, SIGNAL(timeout()), this, SLOT(showManager()));
    connect(actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));  
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit())); 
    connect(taskText->document(), SIGNAL(undoAvailable(bool)),actionUndo, SLOT(setEnabled(bool)));
    connect(taskText->document(), SIGNAL(redoAvailable(bool)),actionRedo, SLOT(setEnabled(bool)));
    actionUndo->setEnabled(taskText->document()->isUndoAvailable());
    actionRedo->setEnabled(taskText->document()->isRedoAvailable());

    connect(actionUndo, SIGNAL(triggered()), taskText, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), taskText, SLOT(redo()));

    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);
    actionSelect_All->setEnabled(false);
    
    connect(actionCut, SIGNAL(triggered()), taskText, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), taskText, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), taskText, SLOT(paste()));
    connect(actionSelect_All, SIGNAL(triggered()), taskText, SLOT(selectAll()));

    connect(taskText, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(taskText, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));
    connect(taskText, SIGNAL(copyAvailable(bool)), actionSelect_All, SLOT(setEnabled(bool)));
    
    
    
    connect(actionSelect_All, SIGNAL(triggered()), taskText, SLOT(selectAll()));
    connect(taskText, SIGNAL(copyAvailable(bool)), actionSelect_All, SLOT(setEnabled(bool)));

    connect(actionTaskmanager, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    
    connect(actionShow_tasks, SIGNAL(triggered()), this, SLOT(showTasks()));
}

void TaskManager::adjustWindow(){
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth, width; 
    int screenHeight, height;
    int x, y;
    QSize windowSize;     
    screenWidth = desktop->width();
    screenHeight = desktop->height();  
    windowSize = size(); 
    width = windowSize.width(); 
    height = windowSize.height();
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;     
    move ( x, y );    
}

void TaskManager::createTryIcon()
{
    trayIconMenu = new QMenu(this);        
    trayIconMenu->addAction(prefsAction);
    trayIconMenu->addSeparator();    
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("images/task.png")); 
    trayIcon->show();
    trayIcon->setContextMenu(trayIconMenu);
}

void TaskManager::createActions()
{
    prefsAction = new QAction(tr("Preferences"), this);
    connect(prefsAction, SIGNAL(triggered()), this, SLOT(showPreferences()));   
    
    restoreAction = new QAction(tr("Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));
    
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void TaskManager::setProjectsList()
{
    taskList->clear();
    QStringList titlesList;
    prefs.db.open();    
    QSqlQuery query;
    query.exec("SELECT DISTINCT title FROM tasksTable");
    int field = query.record().indexOf("title");
    while (query.next()) {
        titlesList << query.value(field).toString();
    }
    titlesList.sort();          
    int currentIndex = titlesList.indexOf(lastTitle);
    taskList->addItems(titlesList);
    if (currentIndex < 0) currentIndex = 0;
    taskList->setCurrentIndex(currentIndex);
}

void TaskManager::setTimes()
{
    QTime now = QTime::currentTime();
    taskStart->setTime(now);
    taskEnd->setTime(now);
}

void TaskManager::save()
{
    lastTitle = taskList->currentText();
    QDate today = QDate::currentDate();
    prefs.db.open();    
    QSqlQuery query;
    QString q = "INSERT INTO tasksTable (title, start, end, detail, taskDate) VALUES ('"+lastTitle+"','"+taskStart->time().toString("HH:mm")+"','"+taskEnd->time().toString("HH:mm")+"','"+taskText->toPlainText()+"','"+today.toString("dd/MM/yyyy")+"')";
    query.exec(q);    
    setProjectsList();    
    prefs.updateList();
}

void TaskManager::startTimer()
{
    prefs.db.open();    
    QSqlQuery query;
    query.exec("SELECT timer FROM prefTable");
    int field = query.record().indexOf("timer");
    query.next();
    int time = query.value(field).toInt();
    timer->start(time*60000);
}

void TaskManager::showManager()
{
    showNormal();
    setWindowState(Qt::WindowActive);
    raise();
}

void TaskManager::showPreferences()
{
    prefs.show();
}

bool TaskManager::tableExists(QString tablename)
{
    QStringList tables = prefs.db.tables(QSql::Tables);
    return tables.indexOf(tablename) >= 0;
}

void TaskManager::createTable(QString tablename)
{
    prefs.db.open();    
    QSqlQuery query;
    if (tablename == "tasksTable")
    {
        query.exec("CREATE TABLE tasksTable (id integer PRIMARY KEY, title string, start string, end string, detail text, taskDate date)");
    }
    else
    {
        query.exec("CREATE TABLE prefTable (id integer, timer integer,  history integer)");
        query.exec("INSERT INTO prefTable VALUES (1,60,30)");
    }    
}

void TaskManager::about()
{
    QString cp = "©";
    QMessageBox::about(this, tr("About Task reminder"),
             "<b> Task reminder application 0.5</b> Copyright " +  QString::fromUtf8(cp.toLatin1().data()) + " 2009 Skander Jabouzi skander@skanderjabouzi.com<br>"
                 " This is a free software distributed under the terms of the GNU General Public License version 3\n(http://www.gnu.org/licenses/gpl-3.0.html)");     
}

void TaskManager::showTasks()
{
    QFile file0("data/.header");    
    file0.open(QIODevice::ReadOnly|QIODevice::Text);
    QString data0 = QString::fromUtf8(file0.readAll());    
    QFile file1("data/.footer");    
    file1.open(QIODevice::ReadOnly|QIODevice::Text);
    QString data1 = QString::fromUtf8(file1.readAll());        
    prefs.db.open();    
    QSqlQuery query;
    query.exec("SELECT * FROM tasksTable");
    QFile file("data/tasks.html");    
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        file.write(data0.toUtf8()); 
        while (query.next()) {
             file.write("<tr>\n");  
             file.write("<td>");   
             file.write(query.value(0).toString().toUtf8());
             file.write("</td>\n");   
             file.write("<td>"); 
             file.write(query.value(1).toString().toUtf8());
             file.write("</td>\n");  
             file.write("<td>");  
             file.write(query.value(2).toString().toUtf8());
             file.write("</td>\n");   
             file.write("<td>"); 
             file.write(query.value(3).toString().toUtf8());
             file.write("</td>\n");  
             file.write("<td>");  
             file.write(query.value(4).toString().toUtf8());
             file.write("</td>\n");   
             file.write("<td>"); 
             file.write(query.value(5).toString().toUtf8());
             file.write("</td>\n");       
             file.write("</tr>\n");  
        }      
        file.write(data1.toUtf8());    
     }
    QDir dir;
    QDesktopServices::openUrl(QUrl("file:///"+dir.currentPath()+"/data/tasks.html"));    
}
//

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <stdlib.h>
#include "log.h"

class Database : public QObject
{  
  Q_OBJECT     
    public:        
        static Database* getInstance();
        ~Database();
        void setDatabaseName(QString);
        void setDatabase();
        void prepareDB(); 
        void setTable(QString); 
        void where(QString); 
        QStringList selectAll(QString);
        QStringList selectAllDistinct(QString);
        QString select(QString);
        void insert(QString); 
        void update(QString, QString);
        QString getStringResult(QString);
        int getIntResult(QString);
        void sqlQuery(QString);
        bool tableExists(QString);
        void createTable(QString);
    
    private:    
        static bool instanceFlag;
        static Database *single;          
        QSqlDatabase db;
        QString database;
        QString table;
        QString sqlWhere;
        QStringList result;  
        QSqlQuery dbQuery;             
        Database(); 
        bool tablesExists();
        void createTables();
        Log *pLog;
};

#endif

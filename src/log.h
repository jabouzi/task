#ifndef LOG_H
#define LOG_H

#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDateTime>

using namespace std;

class Log {
  public:
    Log(const QString&);
    ~Log();
    void Write(QString);
  private:    
    QString fileName;
};

#endif

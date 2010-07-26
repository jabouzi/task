#include "log.h"

Log::Log(const QString &filename) {
  fileName = filename;  
}

void Log::Write(QString logline){  
  QDateTime date = QDateTime::currentDateTime();  
  QString now = date.toString("dd-MM-yyyy hh:mm:ss");
  QFile file(fileName);
  file.open(QFile::WriteOnly | QFile::Append);
  QTextStream streamText(&file);
  streamText.setCodec("UTF-8");
  streamText << now << " -- " <<  logline << endl;
}

Log::~Log(){
  //file.close();
}

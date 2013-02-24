#ifndef SORT
#define SORT
#include <QMainWindow>
#include <QVector>
#include <unistd.h>
#include "ui_Process.h"

class SortInterface: public QMainWindow
{
  Q_OBJECT
    
    private: 
  Ui::Process* _ui;
  QString _vectorString(QVector<unsigned int> inputVector);


 public:
  SortInterface();
  ~SortInterface();
  void setOutputVector(const QString& string);
  void setOutputVector(QVector<unsigned int> vector);
  void setInputVector(QString string);
  void setInputVector(QVector<unsigned int> vector);
  void setPid(int pid);
  void setLeftSonVector(QString string);
  void setLeftSonVector(QVector<unsigned int> vector);
  void setRightSonVector(QString string);
  void setRightSonVector(QVector<unsigned int> vector);


};

#endif

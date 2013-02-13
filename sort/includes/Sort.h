#ifndef SORT
#define SORT
#include <QMainWindow>
#include <unistd.h>
#include "ui_Process.h"

class Sort: public QMainWindow
{
  Q_OBJECT

private:
  QVector<unsigned int> _inputVector;  
  Ui::Process* _ui;
  int _fd;

public:
  Sort(int fd);
  ~Sort();
  void splitVector(QVector<unsigned int>& _splittedVector1, QVector<unsigned int>& _splittedVector2);
  pid_t callChildren(int fd);
};

#endif

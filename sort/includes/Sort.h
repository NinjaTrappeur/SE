#ifndef SORT
#define SORT
#include <QMainWindow>
#include <QVector>
#include <unistd.h>
#include "ui_Process.h"

class Sort: public QMainWindow
{
  Q_OBJECT

private:
  QVector<unsigned int> _inputVector;  
  Ui::Process* _ui;
  int _fd;
  void _saveQVectorToPipe(int fd, QVector<unsigned int>& vector);
  QVector<unsigned int> _readQVectorFromPipe(int fd);
public:
  Sort(int fd);
  ~Sort();
  void splitVector(QVector<unsigned int>& _splittedVector1, QVector<unsigned int>& _splittedVector2);
  pid_t callChildren(int fd);
};

#endif

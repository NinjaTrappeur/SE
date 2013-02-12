#ifndef SORT
#define SORT
#include <QMainWindow>


class Sort: public QMainwindow
{
  Q_OBJECT

private:
  QVector<unsigned int> _inputVector;  
  Ui::Process* ui;
  
public:
  Sort();
  ~Sort();
  
};

#endif

#ifndef MAINWINDOW
#define MAINWINDOW
#include <QMainWindow>
#include <QVector>
#include <QString>
#include "ui_MainWindow.h"

class MainWindow: public QMainWindow
{
  Q_OBJECT
  
private:
  Ui::MainWindow* _ui;
  QVector<unsigned int> _vector;
  
public:
  MainWindow();
  ~MainWindow();
  void generateRandomVector(int size);
  QString vectorString();

public slots:
  void generateNewRandomVector(int size);
  void drawButtonClicked();

signals:
  void vectorSize(int size);
};
#endif

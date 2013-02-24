#ifndef MAINWINDOW
#define MAINWINDOW
#include <QMainWindow>
#include <QVector>
#include <QString>
#include <csignal>
#include "ui_MainWindow.h"

class MainWindow: public QMainWindow
{
  Q_OBJECT
  
private:
  Ui::MainWindow* _ui;
  QVector<unsigned int> _vector;
  pid_t _callChild(int fdWrite[]);
  void _saveQVectorToPipe(int fd, QVector<unsigned int>& vector);
  void _createPipe(int fd[]);
  struct sigaction _action;
  void _initSig();
  
public:
  MainWindow();
  ~MainWindow();
  void generateRandomVector(int size);
  QString vectorString();
  static void sigUsrHandler(int n);

public slots:
  void generateNewRandomVector(int size);
  void drawButtonClicked();
  void launchSort();
  void sendSigUsr();

signals:
  void vectorSize(int size);
};
#endif

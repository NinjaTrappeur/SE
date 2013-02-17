#include <unistd.h>
#include <iostream>
#include <cstdio>
#include "MainWindow.h"
#include "Random.h"

MainWindow::MainWindow()
{
  _ui = new Ui::MainWindow;
  _ui->setupUi(this);
  QObject::connect(_ui->drawButton, SIGNAL(clicked()), _ui->list, SLOT(clear()));
  QObject::connect(_ui->drawButton, SIGNAL(clicked()), this, SLOT(drawButtonClicked()));
  QObject::connect(this, SIGNAL(vectorSize(int)), this, SLOT(generateNewRandomVector(int)));
  QObject::connect(_ui->sortButton, SIGNAL(clicked()), this, SLOT(launchSort()));
  QObject::connect(_ui->stepButton, SIGNAL(clicked()), this, SLOT(sendSigUsr()));
  _initSig();
}

MainWindow::~MainWindow()
{
  delete _ui;
}

QString MainWindow::vectorString()
{
  int i;
  QString string="";
  if(_vector.size()>0)
    {
      for(i=0;i<_vector.size();++i)
	string  += "|" + QString::number(_vector[i]);
      string+="|";
    }
  return string;
}

void MainWindow::generateRandomVector(int size)
{
  Random random;
  _vector=random.generateRandomVector(size,200);
  _ui->list->setText(vectorString());
}

void MainWindow::generateNewRandomVector(int size)
{
  generateRandomVector(size);
}

void MainWindow::drawButtonClicked()
{
  emit vectorSize(_ui->stepSpinBox->value());
}

void MainWindow::launchSort()
{
  int fdRead[2];
  int fdWrite[2];
  _createPipe(fdRead);
  _saveQVectorToPipe(fdRead[1], _vector);
  _createPipe(fdWrite);
  _callChild(fdRead[0],fdWrite[1]);
}

pid_t MainWindow::_callChild(int fdRead, int fdWrite)
{
  pid_t pid;
  switch (pid=fork())
    {
    case -1:
      std::cerr<<"Impossible de forker"<<std::endl;
      exit(-1);
    case 0:
      char fdCRead[100], fdCWrite[100];
      sprintf(fdCRead, "%d", fdRead);
      sprintf(fdCWrite, "%d", fdWrite);
      execlp("./Sort", "./Sort", fdCRead, fdCWrite, (char*)NULL);
    default:
      return pid;
    }
}

void MainWindow::_saveQVectorToPipe(int fd, QVector<unsigned int>& vector)
{
  unsigned int data;
  for(int i=0;i<vector.size();i++)
    {
      data=vector[i];
      if(write(fd, &data, sizeof(unsigned int)) < sizeof(unsigned int))
   	{
	  std::cerr<<"Impossible d'écrire dans le pipe"<<std::endl;
	  exit(-1);
	}
    }
  ::close(fd);
}

void MainWindow::_createPipe(int fd[])
{
  if(pipe(fd) == -1)
    std::cerr<<"Impossible de créer un pipe!"<<std::endl;
}

void MainWindow::sendSigUsr()
{
  killpg(getpgrp(), SIGUSR1);
}

void MainWindow::sigUsrHandler(int n)
{}

void MainWindow::_initSig()
{
  _action.sa_handler = MainWindow::sigUsrHandler;
  sigemptyset(&_action.sa_mask);
  if(sigaction(SIGUSR1, &_action, NULL) == -1) 
    {
      std::cerr<<"Impossible de gérer sigusr1"<<std::endl;
      exit(-1);
    }
}

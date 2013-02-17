#include "Sort.h"
#include <iostream>
#include <cstdio>
#include <unistd.h>

bool Sort::userInterupt=false;

Sort::Sort(int fdRead, int fdWrite):_fdRead(fdRead), _fdWrite(fdWrite), _pid(getpid())
{
  _ui = new Ui::Process;
  _ui->setupUi(this);
  _inputVector=_readQVectorFromPipe(fdRead);
  _ui->entryListLabel->setText(_vectorString());
  _ui->pidLabel->setText("PID: " + QString::number(_pid));
  _initSig();
}

Sort::~Sort()
{
  delete _ui;
}

void Sort::splitVector(QVector<unsigned int>& splittedVector1, QVector<unsigned int>& splittedVector2)
{
  int middle = _inputVector.size()/2; // Le type int tronque les chiffres apres la virgule.
  splittedVector1= _inputVector.mid(0,middle-1);
  splittedVector2= _inputVector.mid(middle, _inputVector.size()-1);
}

pid_t Sort::callChild(int fdRead, int fdWrite)
{
  pid_t pid;
  switch (pid=fork())
    {
    case -1:
      std::cerr<<"Impossible de forker"<<std::endl;
      exit(-1);
    case 0:
      char fdCRead[100], fdCWrite[100] ;
      sprintf(fdCRead, "%d", fdRead);
      sprintf(fdCWrite, "%d", fdWrite);
      execlp("./Sort", "./Sort", fdCRead, fdCWrite, (char*)NULL);
    default:
      return pid;
    }
}

void Sort::_saveQVectorToPipe(int fd, QVector<unsigned int>& vector)
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

QVector<unsigned int> Sort::_readQVectorFromPipe(int fd)
{
  QVector<unsigned int> vector;
  unsigned int data;
  while(read(fd,&data,sizeof(int)) !=0)
    vector.append(data);
  return vector;
}

QString Sort::_vectorString()
{
  QString string="";
  if(_inputVector.size()>0)
    {
      for(int i=0;i<_inputVector.size();++i)
	string  += "|" + QString::number(_inputVector[i]);
      string+="|";
    }
  else
    string="probleme!";
  return string;
}

void Sort::sigUsrHandler(int n)
{
  userInterupt=true;
}

void Sort::_initSig()
{
  _action.sa_handler = Sort::sigUsrHandler;
  sigemptyset(&_action.sa_mask);
  if(sigaction(SIGUSR1, &_action, NULL) == -1) 
    {
      std::cerr<<"Impossible de gérer sigusr1"<<std::endl;
      exit(-1);
    }
}

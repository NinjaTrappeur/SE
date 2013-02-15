#include "Sort.h"
#include <iostream>
#include <cstdio>
#include <unistd.h>

Sort::Sort(int fd):_fd(fd)
{
  _ui = new Ui::Process;
  _ui->setupUi(this);
}

Sort::~Sort()
{
  delete _ui;
}

void Sort::splitVector(QVector<unsigned int>& splittedVector1, QVector<unsigned int>& splittedVector2)
{
  int middle = _inputVector.size()/2; // Le type int tronque les chiffres après la virgule.
  splittedVector1= _inputVector.mid(0,middle-1);
  splittedVector2= _inputVector.mid(middle, _inputVector.size()-1);
}

pid_t Sort::callChildren(int fd)
{
  pid_t pid;
  switch (pid=fork())
    {
    case -1:
      std::cerr<<"Impossible de forker"<<std::endl;
      exit(-1);
    case 0:
      char fdC[100];
      sprintf(fdC, "%d", fd);
      execlp("sort", fdC, (char*)NULL);
    default:
      return pid;
    }
}

void _saveQVectorToPipe(int fd, QVector<unsigned int>& vector)
{
  unsigned int data;
  for(int i=0;i<vector.size();i++)
    {
      data=vector[i];
      if(write(fd, &data, sizeof(unsigned int) < sizeof(unsigned int)))
	{
	  std::cerr<<"Impossible d'écrire dans le pipe"<<std::endl;
	  exit(-1);
	}
    }
}

QVector<unsigned int> _readQVectorFromPipe(int fd)
{
  QVector<unsigned int> vector;
  unsigned int data;
  while(read(fd,&data,sizeof(int)) !=0)
    vector.append(data);
  return vector;
}

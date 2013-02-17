#include <iostream>
#include "SortInterface.h"

SortInterface::SortInterface()
{
  _ui = new Ui::Process;
  _ui->setupUi(this);
}

SortInterface::~SortInterface()
{
  delete _ui;
}

QString SortInterface::_vectorString(QVector<unsigned int> inputVector)
{
  QString string="";
  if(inputVector.size()>0)
    {
      for(int i=0;i<inputVector.size();++i)
	string  += "|" + QString::number(inputVector[i]);
      string+="|";
    }
  else
    string="probleme!";
  return string;
}

void SortInterface::setOutputVector(const QString& string)
{
  _ui->outputList->setText(string);
}

void SortInterface::setOutputVector(QVector<unsigned int> vector)
{
  setOutputVector(_vectorString(vector));
}

void SortInterface::setInputVector(QString string)
{
  _ui->inputList->setText(string);
}

void SortInterface::setInputVector(QVector<unsigned int> vector)
{
  setInputVector(_vectorString(vector));
}

void SortInterface::setPid(int pid)
{
  _ui->pidLabel->setText(QString::number(pid));
}

void SortInterface::setLeftSonVector(QString string)
{
  _ui->leftSonVector->setText(string);
}

void SortInterface::setLeftSonVector(QVector<unsigned int> vector)
{
  setLeftSonVector(_vectorString(vector));
}


void SortInterface::setRightSonVector(QString string)
{
  _ui->rightSonVector->setText(string);
}

void SortInterface::setRightSonVector(QVector<unsigned int> vector)
{
  setRightSonVector(_vectorString(vector));
}

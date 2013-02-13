#include "MainWindow.h"
#include "Random.h"

MainWindow::MainWindow()
{
  _ui = new Ui::MainWindow;
  _ui->setupUi(this);
  QObject::connect(_ui->drawButton, SIGNAL(clicked()), _ui->list, SLOT(clear()));
  QObject::connect(_ui->drawButton, SIGNAL(clicked()), this, SLOT(drawButtonClicked()));
  QObject::connect(this, SIGNAL(vectorSize(int)), this, SLOT(generateNewRandomVector(int)));
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
  emit vectorSize(_ui->stepButton->value());
}


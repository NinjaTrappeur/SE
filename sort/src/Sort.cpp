#include "Sort.h"

Sort::Sort()
{
  _ui = new Ui::Process;
  _ui->setupUi(this);
}

Sort::~Sort()
{
  delete _ui;
}

#include <QApplication>
#include <cstdlib>
#include <iostream>
#include "Sort.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  if(argc<2){
    std::cerr<<"Sort a besoin d'un file descriptor passé en paramètre pour pouvoir être exécuté."<<std::endl;
    return(-1);
  }
  else{
    int fd = atoi(argv[1]);
    Sort* sort = new Sort(fd);
    
    sort->show();
    
    return app.exec();
  }
}

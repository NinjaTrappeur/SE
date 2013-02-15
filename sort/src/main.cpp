#include <QApplication>
#include <cstdlib>
#include <iostream>
#include "Sort.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  //On teste si les deux FD sont bien en paramètres
  if(argc<3){
    std::cerr<<"Sort a besoin de deux file descriptor passé en paramètre pour pouvoir être exécuté."<<std::endl;
    return(-1);
  }
  else{
    //On convertit les deux FD en int
    int fdRead = atoi(argv[1]);
    int fdWrite = atoi(argv[2]);

    Sort* sort = new Sort(fdRead, fdWrite);    
    sort->show();
    return app.exec();
  }
}

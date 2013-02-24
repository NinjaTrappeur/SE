#include <QApplication>
#include <cstdlib>
#include <iostream>
#include "SortInterface.h"
#include "SortEngine.h"


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
    SortInterface* sortInterface = new SortInterface();  
    SortEngine engine(sortInterface, fdRead, fdWrite);
    sortInterface->show();
    return app.exec();
  }
}

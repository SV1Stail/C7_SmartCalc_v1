#include <QApplication>

#include "calc.h"
#include "credit_calc.h"
#include "depos.h"
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  calc w;
  w.show();
  return a.exec();
}

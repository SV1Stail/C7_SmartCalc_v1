#ifndef CALC_H
#define CALC_H

#include <qvariant.h>
#include <qvector.h>

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QString>
#include <cstring>

#include "credit_calc.h"
#include "depos.h"
#define Y_MAX 1000000;
#define Y_MIN -1000000;
#define ALL_SIMBOLS "cossintanacosasinatansqrtlnlog()+-*/%^modx1234567890"
extern "C" {
#include "parser.h"
}
namespace Ui {
class calc;
}

class calc : public QDialog {
  Q_OBJECT

 public:
  explicit calc(QWidget *parent = nullptr);
  ~calc();

 private slots:
  void on_butt_go_to_depos_clicked();
  void on_butt_go_to_credit_clicked();
  void on_butt_exit_clicked();
  void on_butt_solution_clicked();
  int is_x_digit(QString str);
  QString swap_x_on_number(QString str, QString x_str);
  QString swap_x_on_number2(QString str, QString x_str);
  int check_q_iput(QString qstr);

 private:
  Ui::calc *ui;
  depos *depos_window;
  credit_calc *credit_window;
  // x_begin начало промежутка, x_end конец промежутка, h шаг, X временная
  // переменная для заполнения вектора.
  double x_begin, x_end, h, X;
  // n кол-во точек
  int N;
  QVector<double> x, y;
};

#endif  // CALC_H

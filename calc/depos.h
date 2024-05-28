#ifndef DEPOS_H
#define DEPOS_H

#include <qvariant.h>

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QString>
#include <cmath>
#include <ctime>
#include <string>

#include "credit_calc.h"
namespace Ui {
class depos;
}
class depos : public QDialog {
  Q_OBJECT

 public:
  explicit depos(QWidget *parent = nullptr);
  ~depos();

 signals:
  void go_to_calc();

 private slots:
  void on_but_answer_clicked();
  void spisok_popolneniy(int srok);
  void on_butt_calc_clicked();
  void on_butt_exit_clicked();

 private:
  Ui::depos *ui;
};

#endif  // DEPOS_H

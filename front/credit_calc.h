#ifndef CREDIT_CALC_H
#define CREDIT_CALC_H
#include <qvariant.h>

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <cmath>
#include <string>
namespace Ui {
class credit_calc;
}
int check_for_numbers(QString &str);

class credit_calc : public QDialog {
  Q_OBJECT

 public:
  explicit credit_calc(QWidget *parent = nullptr);
  ~credit_calc();

 signals:
  void go_to_calc();
  void go_to_depos();

 private slots:
  void on_pushButton_clicked();

  void on_butt_calc_clicked();

  void on_butt_exit_clicked();

 private:
  Ui::credit_calc *ui;
};

#endif  // CREDIT_CALC_H

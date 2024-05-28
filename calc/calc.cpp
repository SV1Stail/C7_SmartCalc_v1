#include "calc.h"

#include "ui_calc.h"

calc::calc(QWidget *parent) : QDialog(parent), ui(new Ui::calc) {
  ui->setupUi(this);
  depos_window = new depos();
  connect(depos_window, &depos::go_to_calc, this, &calc::show);
  credit_window = new credit_calc();
  connect(credit_window, &credit_calc::go_to_calc, this, &calc::show);
}

calc::~calc() { delete ui; }
void calc::on_butt_go_to_depos_clicked() {
  depos_window->show();
  this->close();
}
void calc::on_butt_go_to_credit_clicked() {
  credit_window->show();
  this->close();
}
void calc::on_butt_exit_clicked() { QApplication::quit(); }

// 1 всё гуд
int calc::is_x_digit(QString str) {
  int flag_p = 0, flag_ret = 0;
  if (!str.contains(QRegExp("[^0-9.()]")) && str[0] != '.') {
    flag_ret = 1;
    for (int i = 0; i < str.length(); i++) {
      if (str[i] == "." && flag_p == 0) {
        flag_p = 1;
      } else if (str[i] == "." && flag_p == 1) {
        flag_ret = 0;
      }
      if (str[i] == "." && str[i + 1] == '\0') {
        flag_ret = 0;
      }
    }
  }
  return flag_ret;
}
QString calc::swap_x_on_number2(QString str, QString x_str) {
  int str_len = str.length(), ret_counter = 1;
  QString ret_str = "(";
  for (int i = 1; i < str_len; i++) {
    if (str[i] != "x") {
      ret_str[ret_counter] = str[i];
      ret_counter++;
    } else {
      ret_str += x_str;
      ret_counter += (int)x_str.length();
    }
  }
  return ret_str;
}

int calc::check_q_iput(QString qstr) {
  QString str = ALL_SIMBOLS;
  for (QChar ch : qstr) {
    if (str.contains(ch) == 0) {
      return 0;
    }
  }
  return 1;
}
QString calc::swap_x_on_number(QString str, QString x_str) {
  if (str.contains('x') == 0)
    return str;
  else {
    int str_len = str.length(), ret_counter = 1;
    QString ret_str = "(";
    for (int i = 1; i < str_len; i++) {
      if (str[i] != "x") {
        ret_str[ret_counter] = str[i];
        ret_counter++;
      } else {
        ret_str += x_str;
        ret_counter += (int)x_str.length();
      }
    }
    return ret_str;
  }
}

void calc::on_butt_solution_clicked() {
  //    stack *head = NULL;

  QString qstr = "(" + ui->line_ask->text() + ")", x_str = ui->line_x->text();

  if (calc::is_x_digit(x_str) && check_q_iput(qstr)) {
    qstr = calc::swap_x_on_number(qstr, x_str);
    QByteArray array = qstr.toLocal8Bit();
    char *input_str = array.data();
    double ans = 0;
    smartcalc(input_str, &ans);
    ui->line_answer->setText(QString::number(ans, 'f', 2));
  } else if (x_str == "NAN" && check_q_iput(qstr)) {
    x.clear();
    y.clear();

    ui->graphic->clearGraphs();
    h = ui->line_h->text().toInt();
    x_begin = Y_MIN;
    x_end = Y_MAX;
    x_end += h;
    ui->graphic->xAxis->setRange(x_begin - 1, x_end + 1);
    ui->graphic->yAxis->setRange(x_begin - 1, x_end + 1);
    X = x_begin;
    N = (x_end - x_begin) / h + 2;

    for (X = x_begin; X <= x_end; X += h) {
      x_str = QString::number(X, 'f', 6);
      x_str = calc::swap_x_on_number2(qstr, x_str);
      QByteArray array = x_str.toLocal8Bit();
      char *input_str = array.data();
      double ans = 0;
      smartcalc(input_str, &ans);
      x.push_back(X);
      y.push_back(ans);
    }
    ui->graphic->addGraph();
    ui->graphic->graph(0)->addData(x, y);
    ui->graphic->replot();
    ui->graphic->setInteraction(QCP::iRangeZoom, true);
    ui->graphic->setInteraction(QCP::iRangeDrag, true);
  }
}

#include "credit_calc.h"

#include "ui_credit_calc.h"

credit_calc::credit_calc(QWidget *parent)
    : QDialog(parent), ui(new Ui::credit_calc) {
  ui->setupUi(this);
}

credit_calc::~credit_calc() { delete ui; }

int check_for_numbers(QString &str) {
  if (str == "") return 1;
  int falg_point = 0, ret = 0;

  for (int i = 0; i < (int)str.length(); i++) {
    if (str[i] == '.' && falg_point == 0) {
      falg_point = 1;
    } else if (str[i] == '.' && falg_point == 1) {
      ret = 1;
    } else if (str[i] != '1' && str[i] != '0' && str[i] != '2' &&
               str[i] != '3' && str[i] != '4' && str[i] != '5' &&
               str[i] != '6' && str[i] != '7' && str[i] != '8' &&
               str[i] != '9') {
      ret = 1;
    }
  }

  return ret;
}

void credit_calc::on_pushButton_clicked() {
  QString text_field_credit_sum = "0", text_field_percent = "0",
          text_field_srok = "0";
  text_field_credit_sum = ui->field_credit_sum->text();
  text_field_percent = ui->field_percent->text();
  text_field_srok = ui->field_srok->text();
  if (check_for_numbers(text_field_credit_sum) ||
      check_for_numbers(text_field_percent) ||
      check_for_numbers(text_field_srok)) {
    QMessageBox::information(this, "ошибка", "проверьте ввод");
  } else {
    double credit_sum = text_field_credit_sum.toDouble(),
           percent = text_field_percent.toDouble();
    int srok = text_field_srok.toInt();
    if (ui->radio_but_annu->isChecked()) {
      //            qDebug() << "annu";

      double every_month_payment_annu =
          credit_sum * (percent / 1200 * pow(percent / 1200 + 1, srok)) /
          (pow(1 + percent / 1200, srok) - 1);
      double pereplata_annu = every_month_payment_annu * srok - credit_sum;
      double all_payment_annu = credit_sum + pereplata_annu;

      //            qDebug() << every_month_payment_annu << " " <<
      //            pereplata_annu << " " << all_payment_annu;
      QString str = QString::number(every_month_payment_annu, 'f', 2);
      ui->field_everymonth_payment->setText(str);
      str = QString::number(all_payment_annu, 'f', 2);
      ui->field_all_priece->setText(str);
      str = QString::number(pereplata_annu, 'f', 2);
      ui->field_pereplata->setText(str);
    } else if (ui->radio_but_diff->isChecked()) {
      //            qDebug() << "diff";
      double every_month_payment_diff_min = credit_sum / srok;
      double every_month_payment_diff_max =
          credit_sum / srok + ((credit_sum * percent / 100 * 31) / 365.0);
      double all_payment_diff = 0, pereplata_diff = 0;
      double summ_left = credit_sum;
      for (int i = 0; i < srok; i++) {
        pereplata_diff += ((summ_left * percent / 100 * 31) / 365.0);
        summ_left = summ_left - (credit_sum / srok +
                                 ((summ_left * percent / 100 * 31) / 365.0));
      }
      all_payment_diff = credit_sum + pereplata_diff;
      //            qDebug() << every_month_payment_diff_max << " " <<
      //            every_month_payment_diff_min << " " << pereplata_diff << " "
      //            << all_payment_diff;
      QString str = QString::number(every_month_payment_diff_min, 'f', 2);
      QString str2 = QString::number(every_month_payment_diff_max, 'f', 2);
      ui->field_everymonth_payment->setText(str2 + " .. " + str);
      str = QString::number(all_payment_diff, 'f', 2);
      ui->field_all_priece->setText(str);
      str = QString::number(pereplata_diff, 'f', 2);
      ui->field_pereplata->setText(str);
    }
  }
}

void credit_calc::on_butt_calc_clicked() {
  this->close();
  emit go_to_calc();
}

void credit_calc::on_butt_exit_clicked() { QApplication::quit(); }

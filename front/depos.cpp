#include "depos.h"

#include "ui_depos.h"
depos::depos(QWidget *parent) : QDialog(parent), ui(new Ui::depos) {
  ui->setupUi(this);
}

depos::~depos() { delete ui; }

void depos::spisok_popolneniy(int srok) {
  time_t t1 = time(NULL);
  tm time_now = *localtime(&t1);
  int counter_mon = 1, year = 0;
  for (int i = 1; i <= srok; i++) {
    int month_int = time_now.tm_mon + counter_mon + 1;  // январь = 0
    if (month_int > 12) {
      month_int -= 12;
      counter_mon -= 12;
      year++;
    }
    counter_mon++;
    QString month_qstring = QString::number(month_int),
            day_qstring = QString::number(time_now.tm_mday),
            year_qstring = QString::number(time_now.tm_year + 1900 + year),
            date_all_qstring =
                day_qstring + "." + month_qstring + "." + year_qstring;
    qDebug() << date_all_qstring;
    ui->lsit_popolneniya->addItem(date_all_qstring);
  }
}

void depos::on_but_answer_clicked() {
  QString text_field_deposit_sum = "0", text_field_percent = "0",
          text_field_srok = "0", text_field_nalog = "0";
  text_field_deposit_sum = ui->field_deposit_sum->text();
  text_field_percent = ui->field_percent->text();
  text_field_srok = ui->field_srok->text();
  text_field_nalog = ui->field_nalog->text();
  ui->lsit_popolneniya->clear();

  if (check_for_numbers(text_field_deposit_sum) ||
      check_for_numbers(text_field_nalog) ||
      check_for_numbers(text_field_percent) ||
      check_for_numbers(text_field_srok)) {
    QMessageBox::critical(this, "error", "Не правильный ввод");
  } else {
    double deposit_sum = text_field_deposit_sum.toDouble();
    double percent = text_field_percent.toDouble();
    double nalog = text_field_nalog.toDouble();
    int srok = text_field_srok.toUInt();
    if (ui->radio_pays_from_bank->isChecked()) {
      double nachislennie_percents =
          (deposit_sum * percent * srok * 30) / 365 / 100;
      double nalog_sum = nachislennie_percents * nalog / 100;
      double deposit_summ_at_the_end =
          nachislennie_percents + deposit_sum - nalog_sum;
      QString str = QString::number(nachislennie_percents, 'f', 2);
      ui->field_nachislenie_percents->setText(str);
      str = QString::number(nalog_sum, 'f', 2);
      ui->field_nalog_sum->setText(str);
      str = QString::number(deposit_summ_at_the_end, 'f', 2);
      ui->field_sum_at_the_end->setText(str);
      time_t t1 = time(NULL);
      tm time_now = *localtime(&t1);
      int year = srok / 12, month = year > 0 ? srok - year * 12 : srok;
      qDebug() << year << month;
      QString date_money = QString::number(time_now.tm_mday) + "." +
                           QString::number(time_now.tm_mon + 1 + month) + "." +
                           QString::number(time_now.tm_year + 1900 + year);
      ui->lsit_popolneniya->addItem(date_money);

    } else {
      double deposit_summ_at_the_end_hard =
          deposit_sum * pow(1 + percent / 100 / 12, srok);
      double nachislennie_percents_hard =
          deposit_summ_at_the_end_hard - deposit_sum;
      double nalog_sum = nachislennie_percents_hard * nalog / 100;
      deposit_summ_at_the_end_hard -= nalog_sum;
      QString str = QString::number(deposit_summ_at_the_end_hard, 'f', 2);
      ui->field_sum_at_the_end->setText(str);
      str = QString::number(nachislennie_percents_hard, 'f', 2);
      ui->field_nachislenie_percents->setText(str);
      str = QString::number(nalog_sum, 'f', 2);
      ui->field_nalog_sum->setText(str);
      spisok_popolneniy(srok);
    }
  }
}

void depos::on_butt_calc_clicked() {
  this->close();
  emit go_to_calc();
}

void depos::on_butt_exit_clicked() { QApplication::quit(); }

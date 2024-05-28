#include <check.h>
#include <math.h>

#include "../front/parser.h"

START_TEST(plus) {
  double ans = 0;
  ck_assert_int_eq(0, smartcalc("3+2", &ans));
  ck_assert_double_eq(5, ans);
}
END_TEST

START_TEST(minus) {
  double ans = 0;
  ck_assert_int_eq(0, smartcalc("3-2", &ans));
  ck_assert_double_eq(1, ans);
}
END_TEST
START_TEST(umn) {
  double ans = 0;
  ck_assert_int_eq(0, smartcalc("3*2", &ans));
  ck_assert_double_eq(6, ans);
}
END_TEST
START_TEST(del) {
  double ans = 0;
  ck_assert_int_eq(0, smartcalc("3/2", &ans));
  ck_assert_double_eq(1.5, ans);
}
END_TEST
START_TEST(vozv) {
  double ans = 0;
  ck_assert_int_eq(0, smartcalc("3^2", &ans));
  ck_assert_double_eq(9, ans);
}
END_TEST

START_TEST(vir1) {
  double an = sin(1) + cos(3), ans = 0;

  ck_assert_int_eq(0, smartcalc("sin(1)+cos(3)", &ans));
  ck_assert_double_eq(an, ans);
}
END_TEST

START_TEST(vir2) {
  double ans = 0;

  ck_assert_int_eq(4, smartcalc("sqrt(4)-acos(3)+tan(1)*asin(2)", &ans));
  ck_assert_double_eq(0, ans);
}
END_TEST

START_TEST(vir3) {
  double ans = 0, an = sqrt(4) + acos(0.5) + tan(0.5) * asin(0.5);

  ck_assert_int_eq(0, smartcalc("sqrt(4)+acos(0.5)+tan(0.5)*asin(0.5)", &ans));
  ck_assert_double_eq(an, ans);
}
END_TEST

START_TEST(vir4) {
  double ans = 0, an = log(1) + atan(0.5);

  ck_assert_int_eq(0, smartcalc("log(1)+atan(0.5)", &ans));
  ck_assert_double_eq(an, ans);
}
END_TEST
START_TEST(vir5) {
  double ans = 0, an = 0;

  ck_assert_int_eq(1, smartcalc("4 mod 2", &ans));
  ck_assert_double_eq(an, ans);
}
END_TEST
START_TEST(vir6) {
  double ans = 0, an = 0;

  ck_assert_int_eq(1, smartcalc("asasfas", &ans));
  ck_assert_double_eq(an, ans);
}
END_TEST
START_TEST(vir7) {
  double ans = 0;

  ck_assert_int_eq(2, smartcalc("4/0", &ans));
  ck_assert_double_eq(0, ans);
}
END_TEST
START_TEST(vir8) {
  double ans = 0;

  ck_assert_int_eq(3, smartcalc("((log(0)))", &ans));
  ck_assert_double_eq(0, ans);
}
END_TEST
START_TEST(vir9) {
  double ans = 0, an = log(5);

  ck_assert_int_eq(0, smartcalc("((ln(5)))", &ans));
  ck_assert_double_eq(an, ans);
}
END_TEST
START_TEST(vir10) {
  double ans = 0, an = log10(5);

  ck_assert_int_eq(0, smartcalc("((log10(5)))", &ans));
  ck_assert_double_eq(an, ans);
}
END_TEST

int main() {
  Suite *suite = suite_create("Unit");
  TCase *tcase = tcase_create("Tests");
  SRunner *srunner = srunner_create(suite);
  int counted_errors = 0;

  suite_add_tcase(suite, tcase);
  tcase_add_test(tcase, plus);
  tcase_add_test(tcase, minus);
  tcase_add_test(tcase, umn);
  tcase_add_test(tcase, del);
  tcase_add_test(tcase, vozv);
  tcase_add_test(tcase, vir1);
  tcase_add_test(tcase, vir2);
  tcase_add_test(tcase, vir3);
  tcase_add_test(tcase, vir4);
  tcase_add_test(tcase, vir5);
  tcase_add_test(tcase, vir6);
  tcase_add_test(tcase, vir7);
  tcase_add_test(tcase, vir8);
  tcase_add_test(tcase, vir9);
  tcase_add_test(tcase, vir10);
  srunner_set_fork_status(srunner, CK_NOFORK);
  srunner_run_all(srunner, CK_VERBOSE);
  counted_errors = srunner_ntests_failed(srunner);
  return counted_errors == 0 ? 0 : 1;
}

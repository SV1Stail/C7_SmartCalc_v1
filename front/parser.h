#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_ERROR 1
#define DIV_BY_ZERO 2
#define LOG_ARG_LE_ZERO 3
#define AFUNC_ARG_OUT_OF_RANGE 4
#define SQRT_ARG_L_ZERO 5
#define M_PI 3.14159265358979323846
typedef struct stack_sc {
  double token;
  int priority;
  int is_num;
  struct stack_sc *next;
} stack_sc;

int detection_errors(double num, char op, int *error_flag);
double make_calc_pref(double num, char op);
double make_calc(double num_1, double num_2, char op);
stack_sc *calculator(stack_sc *polish_notation, int *error_flag, double x_val);
int check_priority(char token);
stack_sc *input_parser(char *input);
stack_sc *dijkstra(stack_sc *lexemes);
int smartcalc(char *input, double *res);
stack_sc *copy_stack(stack_sc *head);
void free_stack(stack_sc **head);
double pop(stack_sc **head);
void push(stack_sc **head, double token, int priority);
int only_one_dot_in_digit(char *input);
int is_only_brackets(char *input);
int check_func_args(char *input, int i, int input_len);
int check_mod(char *input, int i, int input_len);
int missed_operation(char *input);
int unary_ops(char *input, int i, int input_len);
int non_unary_ops(char *input, int i, int input_len);
int is_op_correct_position(char *input);
int is_dot_correct_position(char *input);
int is_brackers_correct(char *input);
int is_x_correct_position(char *input);
int is_x_in_input(char *input);
int validator(char *input, int *error_flag);

#endif  // PARSER_H

#ifndef PARSER_HEADER_H
#define PARSER_HEADER_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_MAX 255
#define IS_DIGIT "-0123456789."
#define STR_ALLOW "^+-*/()sincostgctgsqrtlnx01234567890."
#define IS_ACTION "+*/()^"
#define HEIGHT 100
#define HEIGHT_2 100
#define WIDTH 25
#define IN_STACK "("
#define IN_FUNC "sctgql"
#define IN_OPER "^*/+-sctgql"

typedef struct stack {
  char action;
  double digit;
  struct stack *next;
} stack;
int get_function(char **str, char *ch);
int input(char *str);
int check_br(char *str);
int mb_digit(char **str, double *d);
int prioritet(char c);
int parser(char *str, stack **head);
double get_stack_value(stack *input_lstack_pointer, double x, int *flag);
struct stack *push_s(struct stack **head, char act, double dig);
struct stack *l_add(struct stack **head, char act, double dig);
struct stack *pop_s(struct stack **head, char *act, double *dig);
struct stack *dest_s(struct stack **head);
struct stack *revrs_s(struct stack **in_stack);
stack *get_polish(stack *iput_struct);

#endif  // PARSER_HEADER_H

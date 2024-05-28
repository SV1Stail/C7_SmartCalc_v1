#include "parser_header.h"

struct stack *push_s(struct stack **head, char act, double dig) {
  struct stack *p = (struct stack *)malloc(sizeof(struct stack));
  p->action = act;
  p->digit = dig;
  p->next = NULL;
  if (*head == NULL) {
    *head = p;
  } else {
    struct stack *ptr = *head;
    *head = p;
    p->next = ptr;
  }
  return *head;
}

struct stack *l_add(struct stack **head, char act, double dig) {
  if (act != '\0') {
    struct stack *p = (struct stack *)malloc(sizeof(struct stack));
    p->action = act;
    p->digit = dig;
    p->next = NULL;
    if (*head == NULL) {
      *head = p;
    } else {
      struct stack *ptr = *head;
      while (ptr->next != NULL) {
        ptr = ptr->next;
      }
      ptr->next = p;
    }
  }
  return *head;
}

struct stack *pop_s(struct stack **head, char *act, double *dig) {
  if (*head == NULL) {
  } else {
    *act = (*head)->action;
    *dig = (*head)->digit;
    struct stack *ptr = *head;
    *head = (*head)->next;
    free(ptr);
  }
  return *head;
}

struct stack *dest_s(struct stack **head) {
  struct stack *p = *head;
  while (p != NULL) {
    free(p);
    p = p->next;
  }
  *head = NULL;
  return *head;
}

struct stack *revrs_s(struct stack **in_stack) {
  stack *out_stack = NULL;
  while (*in_stack != NULL) {
    double d = 0;
    char a = '\0';
    pop_s(in_stack, &a, &d);
    push_s(&out_stack, a, d);
  }
  return out_stack;
}
int parser(char *str, stack **head) {
  int flag = 1;
  char ch_old = '\0';
  while (*str && flag) {
    if (*str == 'x') {
      l_add(head, 'x', 0);
    } else if (strchr(IS_ACTION, *str) != NULL) {
      l_add(head, *str, 0);
    } else if (*str == '-') {
      if (ch_old != '\0' && (strchr(IS_ACTION, ch_old) == NULL)) {
        l_add(head, *str, 0); /* Если символ '-' и предыдущий не действие*/
      } else { /*  Если символ '-', получаем цифру и добавляем ее в стек*/
        double d = 0;
        flag = mb_digit(&str, &d);
        l_add(head, 'd', d);
      }
    } else if (strchr(IS_DIGIT, *str) != NULL) {
      double d = 0;
      flag = mb_digit(&str, &d);
      l_add(head, 'd', d);
    } else if (*str != '\n') {
      char ch = '\0';
      flag = get_function(&str, &ch);
      l_add(head, ch, 0);
    }
    ch_old = *str;
    str++;
  }
  if (*head == NULL) {
    flag = 0;
  }
  return flag;
}

int prioritet(char c) {
  int flag = 0;
  switch (c) {
    case '+':
    case '-':
      flag = 1;
      break;
    case '*':
    case '/':
      flag = 2;
      break;
    case '^':
      flag = 3;
      break;
    case 's':
    case 'c':
    case 't':
    case 'g':
    case 'q':
    case 'l':
      flag = 4;
      break;
  }
  return flag;
}

double get_stack_value(stack *input_lstack_pointer, double x, int *flag) {
  stack *stack_for_work = NULL;
  stack *pointer = input_lstack_pointer;
  double d = 0;
  double d2 = 0;
  char a = '\0';
  while (pointer != NULL) {
    switch (pointer->action) {
      case 'd':
        push_s(&stack_for_work, 'd', pointer->digit); /*пушим инт в стек*/
        break;
      case 'x':
        push_s(&stack_for_work, 'x', x); /*пушим значение х в стек*/
        break;
      case '+':
        pop_s(&stack_for_work, &a, &d);
        pop_s(&stack_for_work, &a, &d2);
        push_s(&stack_for_work, 'd', d2 + d);
        break;
      case '*':
        pop_s(&stack_for_work, &a, &d);
        pop_s(&stack_for_work, &a, &d2);
        push_s(&stack_for_work, 'd', d2 * d);
        break;
      case '/':
        pop_s(&stack_for_work, &a, &d);
        pop_s(&stack_for_work, &a, &d2);
        push_s(&stack_for_work, 'd', d2 / d);
        break;
      case '-':
        pop_s(&stack_for_work, &a, &d);
        pop_s(&stack_for_work, &a, &d2);
        push_s(&stack_for_work, 'd', d2 - d);
        break;
      case '^':
        pop_s(&stack_for_work, &a, &d);
        pop_s(&stack_for_work, &a, &d2);
        push_s(&stack_for_work, 'd', pow(d2, d));
        break;
      case 's':
        pop_s(&stack_for_work, &a, &d);
        push_s(&stack_for_work, 'd', sin(d));
        break;
      case 'c':
        pop_s(&stack_for_work, &a, &d);
        push_s(&stack_for_work, 'd', cos(d));
        break;
      case 't':
        pop_s(&stack_for_work, &a, &d);
        push_s(&stack_for_work, 'd', tan(d));
        break;
      case 'g':
        pop_s(&stack_for_work, &a, &d);
        push_s(&stack_for_work, 'd', 1 / tan(d));
        break;
      case 'q':
        pop_s(&stack_for_work, &a, &d);
        push_s(&stack_for_work, 'd', sqrt(d));
        break;
      case 'l':
        pop_s(&stack_for_work, &a, &d);
        push_s(&stack_for_work, 'd', log(d));
        break;
    }
    pointer = pointer->next;
  }
  double fl = 0;
  if (stack_for_work == NULL || stack_for_work->next != NULL) {
    dest_s(&stack_for_work);
    *flag = 0;
  } else {
    fl = stack_for_work->digit;
    dest_s(&stack_for_work);
  }
  return fl;
}
int get_function(char **str, char *ch) {
  int flag = 1;
  char *str_for_find_func = strchr(*str, '(');
  if (str_for_find_func != NULL) {
    int len = (int)(str_for_find_func - *str);
    if (len > 4) {
      flag = 0;
    } else {
      char tempor[STR_MAX] = {'\0'};
      memcpy(tempor, *str, len);
      if (strcmp(tempor, "sin") == 0) {
        *ch = 's';
      } else if (strcmp(tempor, "cos") == 0) {
        *ch = 'c';
      } else if (strcmp(tempor, "tg") == 0) {
        *ch = 't';
      } else if (strcmp(tempor, "ctg") == 0) {
        *ch = 'g';
      } else if (strcmp(tempor, "sqrt") == 0) {
        *ch = 'q';
      } else if (strcmp(tempor, "ln") == 0) {
        *ch = 'l';
      } else {
        flag = 0;
      }
      *str += (len - 1);
    }
  }
  return flag;
}
int input(char *str) {
  int flag = 1;
  char ch = '\0';
  int i = 0;
  while (scanf("%c", &ch) == 1 && strchr(STR_ALLOW, ch) != NULL && flag &&
         ch != '\n' && ch != EOF && ch != -1) {
    if (strchr(STR_ALLOW, ch) != NULL) {
      str[i] = ch;
      i++;
    } else {
      flag = 0;
      break;
    }
  }
  if (ch != '\n') {
    flag = 0;
  }
  return flag;
}

int check_br(char *str) {
  int t = 0;
  for (int i = 0; i < (int)strlen(str); i++) {
    if (str[i] == '(') { /* открывающая скобка, счетчик увеличивается*/
      t++;
    } else if (str[i] == ')') { /*закрывающая скобка, счетчик уменьшается*/
      t--;
      if (t <
          0) /* становится отрицательным, закрывающая скобка без открывающей .*/
        t = -99999;
    }
  }
  return (t == 0) ? 1 : 0;
}
/*
 * Эта функция принимает два параметра: указатель на массив символов и указатель
 * на двойное число. Он возвращает целое число, указывающее, содержит ли массив
 * символов действительное значение типа double.
 */
int mb_digit(char **str, double *d) {
  int flag = 1; /* флаг, указывающий, содержит ли массив символ double*/
  int i = 0;    /*индекс массива символов*/
  int t = 0; /* количество десятичных знаков в массиве символов*/
  while (strchr(IS_DIGIT, *(*str + i)) != NULL && flag) {
    if (i > 0 &&
        *(*str + i) ==
            '-') { /*  символ '-' и это не первый символ, выйти из цикла*/
      break;
    }
    if (*(*str + i) ==
        '.') { /*  символ '.', увеличиваем количество десятичных знаков*/
      t++;
      if (t > 1) { /* знаков после запятой больше одной, устанавливаем флаг*/
        flag = 0;
      }
    }
    i++;
  }
  char tempor[STR_MAX] = {'\0'};
  memcpy(tempor, *str, i);
  if (sscanf(tempor, "%lf", d) != 1) {
    flag = 0;
  }
  *str += (i - 1);
  return flag;
}

int x(char **str, double *d) {
  int flag = 1; /* флаг, указывающий, содержит ли массив символ double*/
  int i = 0;    /*индекс массива символов*/
  int t = 0; /* количество десятичных знаков в массиве символов*/
  while (strchr(IS_DIGIT, *(*str + i)) != NULL && flag) {
    if (i > 0 &&
        *(*str + i) ==
            '-') { /*  символ '-' и это не первый символ, выйти из цикла*/
      break;
    }
    if (*(*str + i) ==
        '.') { /*  символ '.', увеличиваем количество десятичных знаков*/
      t++;
      if (t > 1) { /* знаков после запятой больше одной, устанавливаем флаг*/
        flag = 0;
      }
    }
    i++;
  }
  char tempor[STR_MAX] = {'\0'};
  memcpy(tempor, *str, i);
  if (sscanf(tempor, "%lf", d) != 1) {
    flag = 0;
  }
  *str += (i - 1);
  return flag;
}
stack *get_polish(stack *iput_struct) {
  double d = 0;
  char a = '\0';
  stack *iput_struct_list = NULL;
  stack *out_struct_list = NULL;
  if (iput_struct != NULL) {
    stack *pointer = iput_struct;
    while (pointer != NULL) {
      if (pointer->action == 'd' || pointer->action == 'x') {
        push_s(&out_struct_list, pointer->action, pointer->digit);
      } else if (strchr(IN_STACK, pointer->action) != NULL) {
        push_s(&iput_struct_list, pointer->action, pointer->digit);
      } else if (pointer->action == ')') {
        if (iput_struct_list != NULL) {
          do {
            pop_s(&iput_struct_list, &a, &d);
            if (a != '(') {
              push_s(&out_struct_list, a, d);
            }
          } while (a != '(' && iput_struct_list != NULL);
          if (iput_struct_list != NULL &&
              strchr(IN_FUNC, iput_struct_list->action) != NULL) {
            pop_s(&iput_struct_list, &a, &d);
            push_s(&out_struct_list, a, d);
          }
        }
      } else if (strchr(IN_OPER, pointer->action) != NULL) {
        if (iput_struct_list != NULL) {
          while (iput_struct_list != NULL &&
                 prioritet(iput_struct_list->action) >=
                     prioritet(pointer->action)) {
            pop_s(&iput_struct_list, &a, &d);
            push_s(&out_struct_list, a, d);
          }
        }
        push_s(&iput_struct_list, pointer->action, pointer->digit);
      }
      pointer = pointer->next;
    }
    while (iput_struct_list != NULL) {
      pop_s(&iput_struct_list, &a, &d);
      push_s(&out_struct_list, a, d);
    }
  }
  dest_s(&iput_struct_list);
  return out_struct_list;
}

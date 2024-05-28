#include "parser.h"
int validator(char *input, int *error_flag) {
  int result = 0;
  result += strlen(input) == 0;
  is_x_in_input(input);  // X -> x
  result += is_brackers_correct(input);
  result += is_x_correct_position(input);
  result += is_dot_correct_position(input);
  result += is_op_correct_position(input);
  result += is_only_brackets(input);
  result += only_one_dot_in_digit(input);
  result += missed_operation(input);
  if (result != 0) *error_flag = 1;
  return result;
}

int is_x_in_input(char *input) {
  int input_len = strlen(input);
  int is_x = 0;
  for (int i = 0; i < input_len; i++) {
    if (input[i] == 'X' || input[i] == 'x') {
      input[i] = 'x';
      is_x = 1;
    }
  }
  return is_x;
}

int is_x_correct_position(char *input) {
  int input_len = strlen(input);
  int flag = 0;
  for (int i = 0; i < input_len; i++) {
    if (i > 0 && i < input_len - 1 &&
        input[i] == 'x') {  // Если х не находится на крайних позициях
      flag += isdigit(input[i - 1]) || input[i - 1] == 'x';
      flag += isdigit(input[i + 1]) || input[i + 1] == 'x';
    } else if (i == 0 && input[i] == 'x') {  // Если х находится в начале строки
      flag += isdigit(input[i + 1]) || input[i + 1] == 'x';
    } else if (i == input_len - 1 &&
               input[i] == 'x') {  // Если х находится в конце строки
      flag += isdigit(input[i - 1]) || input[i - 1] == 'x';
    }
  }
  return flag;
}

int is_brackers_correct(char *input) {
  int flag = 0;
  int input_len = strlen(input);
  for (int pos = 0; pos < input_len && flag >= 0; pos++) {
    if (input[pos] == '(') {
      flag++;
    } else if (input[pos] == ')') {
      flag--;
    }
  }
  return flag;
}

int is_dot_correct_position(char *input) {
  int input_len = strlen(input);
  int flag = 0;
  for (int i = 0; i < input_len; i++) {
    // Если точка находится не между цифрами или точка стоит на самом первом
    // или на самом последнем месте, значит некорректный ввод
    if ((input[i] == '.' && i > 0 && i < input_len - 1 &&
         (isdigit(input[i - 1]) == 0 || isdigit(input[i + 1]) == 0)) ||
        (input[i] == '.' && (i == 0 || i == input_len - 1))) {
      flag++;
    }
  }
  return flag;
}

int is_op_correct_position(char *input) {
  int input_len = strlen(input);
  int flag = 0;
  for (int i = 0; i < input_len; i++) {
    if (input[i] != '.' && !isdigit(input[i]) && input[i] != 'x') {
      flag += non_unary_ops(input, i, input_len);
      flag += unary_ops(input, i, input_len);
      flag += check_mod(input, i, input_len);
    }
    if (input[i] == 's' || input[i] == 'a' || input[i] == 't' ||
        input[i] == 'l' || input[i] == 'c' || input[i] == '(') {
      flag += check_func_args(input, i, input_len);
    }
  }
  return flag;
}

int non_unary_ops(char *input, int i, int input_len) {
  char operators[] = {'*', '/', '^'};
  int flag = 0;

  for (int j = 0; j < 3; j++) {
    // Если нынешний токен - оператор, и при этом не находится на крайней
    // позиции, то проверяем окружающие его лексеммы на число, бинарные знаки
    // или x
    if (input[i] == operators[j] && (i != 0 && i != input_len - 1)) {
      // Если окружающие токены - скобки, то флаг учтет это
      flag +=
          (!isdigit(input[i - 1]) && input[i - 1] != 'x' &&
           input[i - 1] != 'c' && input[i - 1] != 't' && input[i - 1] != 'l' &&
           input[i - 1] != 'a' && input[i - 1] != 's' && input[i - 1] != ')') +
          (!isdigit(input[i + 1]) && input[i + 1] != 'x' &&
           input[i + 1] != 'c' && input[i + 1] != 't' && input[i + 1] != 'l' &&
           input[i + 1] != 'a' && input[i + 1] != 's' && input[i + 1] != '(' &&
           input[i + 1] != '-' && input[i + 1] != '+');
    } else if (input[i] == operators[j] && (i == 0 || i == input_len - 1)) {
      flag++;
    }
  }
  return flag;
}

int unary_ops(char *input, int i, int input_len) {
  char unar_operators[] = {'-', '+'};
  char unar_operators_for_change[] = {'~', '#'};
  int flag = 0;
  // В этом цикле проверяются лексеммы операций, которые могут быть
  // унарными, если это подтвердится, то '-' заменится на '~', а '+' на '#'
  // для дальнейшего парсинга
  for (int j = 0; j < 2; j++) {
    // Если нынешний токен - оператор, и при этом не находится на крайней
    // правой позиции, то проверяем лексемму справа на число, унарные знаки
    // или x
    if (input[i] == unar_operators[j] && i != input_len - 1) {
      // Если окружающие токены - скобки, то флаг учтет это
      flag += !isdigit(input[i + 1]) && input[i + 1] != 'x' &&
              input[i + 1] != '(' && input[i + 1] != '-' &&
              input[i + 1] != 'c' && input[i + 1] != 't' &&
              input[i + 1] != 'l' && input[i + 1] != 'a' &&
              input[i + 1] != 's' && input[i + 1] != '+';
    } else if (input[i] == unar_operators[j] && i == input_len - 1) {
      flag++;
    }
    // Тут происходит замена унарных знаков
    if (input[i] == unar_operators[j] &&
        (i == 0 ||
         (!isdigit(input[i - 1]) && input[i - 1] != 'x' &&
          input[i - 1] != 'c' && input[i - 1] != 't' && input[i - 1] != 'l' &&
          input[i - 1] != 'a' && input[i - 1] != 's' && input[i - 1] != ')'))) {
      input[i] = unar_operators_for_change[j];
    }
  }
  return flag;
}

int missed_operation(char *input) {
  int flag = 0;
  int input_len = strlen(input);
  char operations[] = {'+', '-', '*', '/', '^', 'm', ')'};
  int operations_len = strlen(operations);
  for (int i = 0; i < input_len; i++) {
    if (i < input_len - 2 && input[i] == ')') {
      int match_counter = 0;
      for (int j = 0; j < operations_len; j++) {
        if (input[i + 1] != operations[j]) match_counter++;
      }
      flag += match_counter == 7;
    }
  }
  return flag;
}

int check_mod(char *input, int i, int input_len) {
  // В этом условии будет проверяться корректность ввода mod
  int flag = 0;
  if (input[i] == 'm' && i != 0 && i != input_len - 3) {
    flag +=
        (!isdigit(input[i - 1]) && input[i - 1] != 'x' && input[i - 1] != ')' &&
         input[i - 1] != 'c' && input[i - 1] != 't' && input[i - 1] != 'l' &&
         input[i - 1] != 'a' && input[i - 1] != 's') +
        (!isdigit(input[i + 3]) && input[i + 3] != 'x' && input[i + 3] != '(' &&
         input[i + 3] != 'c' && input[i + 3] != 't' && input[i + 3] != 'l' &&
         input[i + 3] != 'a' && input[i + 3] != 's');
  } else if (input[i] == 'm' && (i == 0 || i == input_len - 3)) {
    flag++;
  }
  return flag;
}

int check_func_args(char *input, int i, int input_len) {
  while (input[i] != '(' && i < input_len) i++;
  return i >= input_len - 1 || input[i + 1] == ')';
}

int is_only_brackets(char *input) {
  int input_len = strlen(input);
  int flag = input_len;
  for (int i = 0; i < input_len; i++) {
    if (isdigit(input[i]) || input[i] == 'x') flag--;
  }
  return flag == input_len;
}

int only_one_dot_in_digit(char *input) {
  int input_len = strlen(input);
  int dot_flag = 0;
  int flag = 0;
  for (int i = 0; i < input_len; i++) {
    dot_flag = 0;
    if (isdigit(input[i])) {
      while (i < input_len && (isdigit(input[i]) || input[i] == '.')) {
        i++;
        if (input[i] == '.') dot_flag++;
      }
    }
    if (dot_flag > 1) flag++;
  }
  return flag;
}

double pop(stack_sc **head) {
  stack_sc *out;
  if (*head == NULL) {
    exit(1);
  }

  double value = 0;
  out = *head;
  *head = (*head)->next;
  value = out->token;
  free(out);
  return value;
}

void free_stack(stack_sc **head) {
  while (*head != NULL) {
    pop(head);
  }
}

stack_sc *copy_stack(stack_sc *head) {
  stack_sc *copy = {0};
  stack_sc *reverse_copy = {0};
  while (head) {
    push(&copy, head->token, head->priority);
    copy->is_num = head->is_num;
    head = head->next;
  }

  while (copy) {
    int priority = copy->priority;
    if (copy->is_num == 1 || copy->is_num == 2) {
      int num_flag = copy->is_num;
      push(&reverse_copy, pop(&copy), priority);
      reverse_copy->is_num = num_flag;
    } else {
      push(&reverse_copy, pop(&copy), priority);
    }
  }

  return reverse_copy;
}
int smartcalc(char *input, double *res) {
  int error_flag = 0;
  if (validator(input, &error_flag) == 0) {
    stack_sc *lexemes = input_parser(input);
    stack_sc *polish_notation = dijkstra(lexemes);
    stack_sc *result = calculator(polish_notation, &error_flag, 0);
    if (error_flag == 0) {
      *res = pop(&result);
      if ((*res < 1e-7 && *res > 0) || (*res > -1e-7 && *res < 0)) *res = 0;
    }
  }
  return error_flag;
}
stack_sc *dijkstra(stack_sc *lexemes) {
  stack_sc *result = {0};
  stack_sc *operations = {0};

  while (lexemes) {
    if (lexemes->is_num == 1 || lexemes->is_num == 2) {
      int priority = lexemes->priority;
      int is_num = lexemes->is_num;
      push(&result, pop(&lexemes), priority);
      result->is_num = is_num;
    } else if (lexemes->priority == 4) {
      int priority = lexemes->priority;
      push(&operations, pop(&lexemes), priority);
    } else if (lexemes->token == '(') {
      int priority = lexemes->priority;
      push(&operations, pop(&lexemes), priority);
    } else if (lexemes->token == ')') {
      while (operations && operations->token != '(') {
        int priority = operations->priority;
        push(&result, pop(&operations), priority);
      }
      pop(&operations);
      pop(&lexemes);
    } else if (operations && (lexemes->priority <= operations->priority)) {
      while (operations && (lexemes->priority <= operations->priority)) {
        int priority = operations->priority;
        push(&result, pop(&operations), priority);
      }
      int priority = lexemes->priority;
      push(&operations, pop(&lexemes), priority);
    } else {
      int priority = lexemes->priority;
      push(&operations, pop(&lexemes), priority);
    }
  }

  while (operations) {
    int priority = operations->priority;
    push(&result, pop(&operations), priority);
  }

  stack_sc *reverse_result = {0};
  while (result) {
    int priority = result->priority;
    int num_flag = result->is_num;
    push(&reverse_result, pop(&result), priority);
    reverse_result->is_num = num_flag;
  }
  return reverse_result;
}

stack_sc *input_parser(char *input) {
  stack_sc *parse = {0};
  int len_input = strlen(input);
  int unar_minus_flag = 0;
  for (int i = 0; i < len_input; i++) {
    if (isdigit(input[i])) {
      char buff[255] = {0};
      for (int j = 0;
           j < 255 && (isdigit(input[i]) || input[i] == '.') && i < len_input;
           j++, i++) {
        buff[j] = input[i];
      }
      double num = 0;
      sscanf(buff, "%lf", &num);
      if (unar_minus_flag == 1) {
        num *= -1;
        unar_minus_flag = 0;
      }
      push(&parse, num, 0);
      parse->is_num = 1;
    }
    if (input[i] == '+' || input[i] == '-' || input[i] == '*' ||
        input[i] == '/' || input[i] == '^' || input[i] == '-' ||
        input[i] == ')' || input[i] == '(') {
      push(&parse, input[i], check_priority(input[i]));
    }
    if (input[i] == 'X' || input[i] == 'x') {
      push(&parse, input[i], check_priority(input[i]));
      parse->is_num = 2;
    }
    if (input[i] == '~') {
      unar_minus_flag = 1;
    }
    if (input[i] == 'm' && input[i + 1] == 'o' && input[i + 2] == 'd') {
      push(&parse, 'm', check_priority(input[i]));
      i += 2;
    }
    if (input[i] == 'c' && input[i + 1] == 'o' && input[i + 2] == 's') {
      push(&parse, 'c', check_priority(input[i]));
      i += 2;
    }
    if (input[i] == 't' && input[i + 1] == 'a' && input[i + 2] == 'n') {
      push(&parse, 't', check_priority(input[i]));
      i += 2;
    }
    if (i < len_input - 3 && input[i] == 's' && input[i + 1] == 'i' &&
        input[i + 2] == 'n') {
      push(&parse, 's', check_priority(input[i]));
      i += 2;
    }
    if (i < len_input - 4 && input[i] == 's' && input[i + 1] == 'q' &&
        input[i + 2] == 'r' && input[i + 3] == 't') {
      push(&parse, 'q', check_priority(input[i]));
      i += 3;
    }
    if (input[i] == 'a' && input[i + 1] == 's' && input[i + 2] == 'i' &&
        input[i + 3] == 'n') {
      push(&parse, 'n', check_priority(input[i]));
      i += 3;
    }
    if (input[i] == 'a' && input[i + 1] == 'c' && input[i + 2] == 'o' &&
        input[i + 3] == 's') {
      push(&parse, 'o', check_priority(input[i]));
      i += 3;
    }
    if (input[i] == 'a' && input[i + 1] == 't' && input[i + 2] == 'a' &&
        input[i + 3] == 'n') {
      push(&parse, 'a', check_priority(input[i]));
      i += 3;
    }
    if (input[i] == 'l' && input[i + 1] == 'o' && input[i + 2] == 'g') {
      push(&parse, 'g', check_priority(input[i]));
      i += 2;
    }
    if (input[i] == 'l' && input[i + 1] == 'n') {
      push(&parse, 'l', check_priority(input[i]));
      i++;
    }
  }
  stack_sc *reverse_parse = {0};
  while (parse) {
    int priority = parse->priority;
    if (parse->is_num == 1) {
      push(&reverse_parse, pop(&parse), priority);
      reverse_parse->is_num = 1;
    } else if (parse->is_num == 2) {
      push(&reverse_parse, pop(&parse), priority);
      reverse_parse->is_num = 2;
    } else {
      push(&reverse_parse, pop(&parse), priority);
    }
  }

  return reverse_parse;
}

int check_priority(char token) {
  int priority = 0;
  switch (token) {
    case 'x':
      priority = 0;
      break;
    case ')':
      priority = 0;
      break;
    case '(':
      priority = 0;
      break;
    case '+':
      priority = 1;
      break;
    case '-':
      priority = 1;
      break;
    case '*':
      priority = 2;
      break;
    case '/':
      priority = 2;
      break;
    case 'm':
      priority = 2;
      break;
    case '^':
      priority = 3;
      break;
    default:
      priority = 4;
      break;
  }
  return priority;
}
stack_sc *calculator(stack_sc *polish_notation, int *error_flag, double x_val) {
  stack_sc *operations = {0};
  while (polish_notation) {
    if (polish_notation->is_num == 1) {
      int priority = polish_notation->priority;
      push(&operations, pop(&polish_notation), priority);
      operations->is_num = 1;
    } else if (polish_notation->is_num == 2) {
      int priority = polish_notation->priority;
      pop(&polish_notation);
      push(&operations, x_val, priority);
      operations->is_num = 2;
    } else if (polish_notation->priority != 4) {
      char op = pop(&polish_notation);
      double num_2 = pop(&operations);
      double num_1 = pop(&operations);
      if (detection_errors(num_2, op, error_flag) == 0) {
        double result = make_calc(num_1, num_2, op);
        push(&operations, result, 0);
        operations->is_num = 1;
      } else {
        if (operations) free_stack(&operations);
        if (polish_notation) free_stack(&polish_notation);
      }
    } else {
      char op = pop(&polish_notation);
      double num = pop(&operations);
      if (detection_errors(num, op, error_flag) == 0) {
        double result = make_calc_pref(num, op);
        push(&operations, result, 0);
        operations->is_num = 1;
      } else {
        if (operations) free_stack(&operations);
        if (polish_notation) free_stack(&polish_notation);
      }
    }
  }
  return operations;
}

double make_calc(double num_1, double num_2, char op) {
  double result = 0;
  switch (op) {
    case '+':
      result = num_1 + num_2;
      break;
    case '-':
      result = num_1 - num_2;
      break;
    case '*':
      result = num_1 * num_2;
      break;
    case '/':
      result = num_1 / num_2;
      break;
    case '^':
      result = pow(num_1, num_2);
      break;
    case 'm':
      result = fmod(num_1, num_2);
      break;
  }
  return result;
}

double make_calc_pref(double num, char op) {
  double result = 0;
  switch (op) {
    case 's':
      result = sin(num);
      break;
    case 'q':
      result = sqrt(num);
      break;
    case 'n':
      result = asin(num);
      break;
    case 'o':
      result = acos(num);
      break;
    case 'a':
      result = atan(num);
      break;
    case 'l':
      result = log(num);
      break;
    case 'g':
      result = log10(num);
      break;
    case 't':
      result = tan(num);
      break;
    case 'c':
      result = cos(num);
      break;
  }
  return result;
}

int detection_errors(double num, char op, int *error_flag) {
  int flag = 0;

  if ((op == '/' || op == 'm') && num == 0) {
    *error_flag = DIV_BY_ZERO;
    flag++;
  } else if ((op == 'l' || op == 'g') && num <= 0.0) {
    *error_flag = LOG_ARG_LE_ZERO;
    flag++;
  } else if ((op == 'o' || op == 'n') && (num < -1.0 || num > 1.0)) {
    *error_flag = AFUNC_ARG_OUT_OF_RANGE;
    flag++;
  } else if (op == 'q' && num < 0) {
    *error_flag = SQRT_ARG_L_ZERO;
    flag++;
  }

  return flag;
}

void push(stack_sc **head, double token, int priority) {
  stack_sc *tmp = calloc(1, sizeof(stack_sc));

  if (tmp == NULL) {
    exit(1);
  }

  tmp->next = *head;
  tmp->token = token;
  tmp->priority = priority;
  *head = tmp;
}

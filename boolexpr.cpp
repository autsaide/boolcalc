#include "boolexpr.h"


bool BooleanExpression::priority(const char a, const char b) {
  const char* priority = "~&+v><=|^";
  for (auto i = priority[0]; i!='\0'; i++ ){
    if (a == i) {
      return true;
    } else if (b == i) {
      return false;
    }
  }
}

void BooleanExpression::calculate(std::stack<char> &operators, std::stack<bool> &values) {
  bool tmp;
  switch (operators.top()) {
    case '&': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = first && second;
    }
    case 'v': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = first || second;
    }
    case '~': {
      bool first = values.top();
      values.pop();
      tmp = !first;
      break;
    }
    case '>': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = first <= second;
      break;
    }
    case '<': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = first >= second;
      break;
    }
    case '+': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = first ^ second;
      break;
    }
    case '=': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = first == second;
      break;
    }
    case '|': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = !(first && second);
      break;
    }
    case '^': {
      bool second = values.top();
      values.pop();
      bool first = values.top();
      values.pop();
      tmp = !(first || second);
      break;
    }
  }
  values.pop();
  values.push(tmp);
}

bool BooleanExpression::calc(const char * string) {
  std::stack<char> operators;
  std::stack<bool> values;


  for (auto i = string[0]; i != '\0'; i++){
    switch (i) {
      case '&':
      case 'v':
      case '~':
      case '>':
      case '<':
      case '+':
      case '=':
      case '|':
      case '^': {
        if (priority(operators.top(), i)) {
          calculate(operators, values);
        }
        operators.push(i);
        break;
      }
      case '0': {
        values.push(false);
        break;
      }
      default: {
        values.push(true);
      }
    }

  }
  while (!operators.empty()) {
    calculate(operators, values);
  }
}

/*
 *
 * & конъюнкция
v дизъюнкция
~ отрицание
> импликация (x → y)
< обратная импликация (x ← y)
+ сложение по модулю 2
= эквиваленция
| штрих Шеффера
^ стрелка Пирса
 */
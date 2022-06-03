#pragma once
#include <stack>

class BooleanExpression{
 private:
  static bool priority(const char a, const char b);
  static void calculate(std::stack<char> &operators, std::stack<bool> &values);
 public:
  BooleanExpression(const char*);
  BooleanExpression(const BooleanExpression &);
  bool calc(const char * string);
  void truthTable();

};


#pragma once
#include <stack>
#include "node.h"
#include <sstream>
#include <cmath>
#include <iomanip>
#include <vector>

class BooleanExpression {
  calculator::Node *expression_;
  std::string string_;
  bool *truth_table_ = nullptr;
  bool *zhegalkin_ = nullptr;
  uint32_t size_;

  //
  // Internal methods
  //
 private:
  static calculator::Node *ParseString(const char *string);
  static bool Priority(const char a, const char b);
  static calculator::Node *ParseNode(std::stack<calculator::Node *> &numbers, std::stack<char> &operators);
  static void IncrementVariables(std::map<unsigned int, bool> &vars);
  std::string BuildNormalForm(bool And);
  void GenerateTruthTable();
  void GenerateZhegalkin();

  //
  // Public methods
  //
 public:
  BooleanExpression(const char *);
  ~BooleanExpression();
  void RecalculateString();
  bool calc();
  bool calc(calculator::Context &ctx);
  void truthTable();
  BooleanExpression cnf();
  BooleanExpression dnf();
  BooleanExpression zhegalkin();
  unsigned char Mask();

  operator std::string () const;

};

static bool IsFull(std::vector<BooleanExpression> &expressions){
  unsigned char cl = 0b11111;
  for (auto &exp : expressions) {
    cl = cl & exp.Mask();
  }
  return cl == 0;
}

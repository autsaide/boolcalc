#include "boolexpr.h"

//
// Internal methods
//

calculator::Node *BooleanExpression::ParseString(const char *string) {
  std::stack <calculator::Node *> numbers;
  std::stack<char> operators;

  std::stringstream stream(string);
  for (char character = stream.get(); !stream.eof(); character = stream.get()) {
    switch (character) {
      case ' ': {   // Empty line
        continue;
      }
      case '1':     // Constants
      case '0': {
        numbers.push(new calculator::NumberNode(character - '0'));
        break;
      }
      case 'x': {   // Variable
        unsigned int id;
        stream >> id;
        numbers.push(new calculator::VarNode(id));
        break;
      }
      default: {    // Operator
        bool skip = false;
        while (!operators.empty()) {
          if (character == '(')
            break;
          if (character == ')' && operators.top() == '(') {
            operators.pop();
            skip = true;
          }

          if (Priority(operators.top(), character))
            break;
          ParseNode(numbers, operators);
        } // while
        if (!skip)
          operators.push(character);
      } // default

    } // switch
  } // for

  while (operators.size() != 0)
    ParseNode(numbers, operators);
  return numbers.top();
}

bool BooleanExpression::Priority(const char a, const char b) {
  const char priority[] = ")~&+v><=|^(";
  for (auto i = priority; *i != '\0'; ++i) {
    if (a == *i)
      return false;
    if (b == *i)
      return true;
  }
  return false;
}

calculator::Node *BooleanExpression::ParseNode(std::stack<calculator::Node *> &numbers, std::stack<char> &operators) {
  calculator::Node *new_node = nullptr;
  switch (operators.top()) {
    case '&':
    case 'v':
    case '>':
    case '<':
    case '+':
    case '=':
    case '|':
    case '^': {   // Binary nodes
      calculator::Node *a = numbers.top();
      numbers.pop();
      calculator::Node *b = numbers.top();
      numbers.pop();
      switch (operators.top()) {
        case '&': {
          new_node = new calculator::AndNode(a, b);
          break;
        }
        case 'v': {
          new_node = new calculator::OrNode(a, b);
          break;
        }
        case '>': {
          new_node = new calculator::ImplNode(a, b);
          break;
        }
        case '<': {
          new_node = new calculator::RevImplNode(a, b);
          break;
        }
        case '+': {
          new_node = new calculator::XorNode(a, b);
          break;
        }
        case '=': {
          new_node = new calculator::EqNode(a, b);
          break;
        }
        case '|': {
          new_node = new calculator::ShefferNode(a, b);
          break;
        }
        case '^': {
          new_node = new calculator::PierceNode(a, b);
          break;
        }
      } // operators switch
      break;
    } // Binary nodes case
    case '~': { // Unary nodes
      calculator::Node *a = numbers.top();
      numbers.pop();
      new_node = new calculator::NegNode(a);
      break;
    }
  } // Binary/Unary switch

  if (new_node != nullptr)
    numbers.push(new_node);
  operators.pop();
  return new_node;
}

void BooleanExpression::IncrementVariables(std::map<unsigned int, bool> &vars) {
  for (auto &a : vars) {
    if (!a.second) {
      a.second = true;
      break;
    } else {
      a.second = false;
    }
  }
}

std::string BooleanExpression::BuildNormalForm(bool And) {
  auto ctx = calculator::FakeContext(expression_);

  std::string res = "(";
  for (int i = 0; i < 1ULL << size_; ++i) {
    if (expression_->calc(ctx) != And) {
      for (auto a : ctx.vars)
        res += std::string(a.second ? "" : "~") + "x" + std::to_string(a.first) + (And ? " v " : " & ");
      res.erase(res.end() - 3, res.end());
      res += ") " + std::string(And ? "&" : "v")+ " (";
    }
    IncrementVariables(ctx.vars);
  }
  res.erase(res.end() - 3, res.end());
  return res;
}

void BooleanExpression::GenerateTruthTable() {
  if (truth_table_ != nullptr)
      delete[] truth_table_;
  auto ctx = calculator::FakeContext(expression_);
  truth_table_ = new bool[1ULL << size_];
  for (int i = 0; i < 1ULL << size_; ++i) {
    truth_table_[i] = expression_->calc(ctx);
    IncrementVariables(ctx.vars);
  }
}

void BooleanExpression::GenerateZhegalkin() {
  if (zhegalkin_ != nullptr)
      delete[] zhegalkin_;
  zhegalkin_ = new bool[1ULL << size_];
  auto ctx = calculator::FakeContext(expression_);

  const auto &N = size_;

  bool **table = new bool *[2];
  for (int i = 0; i < 2; ++i)
    table[i] = new bool[1ULL << N];

  for (int i = 0; i < 1ULL << N; ++i) {
    table[0][i] = expression_->calc(ctx);
    IncrementVariables(ctx.vars);
  }

  bool flag = true;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < (1ULL << N); ++j) {
      if (!(j % (1ULL << i)))
        flag = !flag;
      if (flag) {
        table[1][j] = (table[0][j] != table[0][j - (1ULL << (i))]);
      } else {
        table[1][j] = table[0][j];
      }
    }
    for (int j = 0; j < (1ULL << N); ++j)
      table[0][j] = table[1][j];

  }
  for (int j = 0; j < (1ULL << N); ++j)
    zhegalkin_[j] = table[0][j];
  delete[] table[0];
  delete[] table[1];
  delete[] table;
}

//
// Public methods
//

BooleanExpression::BooleanExpression(const char *string) {
  expression_ = ParseString(string);
  string_ = string;
  auto ctx = calculator::FakeContext(expression_);
  size_ = ctx.vars.size();
}

BooleanExpression::~BooleanExpression() {
  delete expression_;
  delete[] truth_table_;
  delete[] zhegalkin_;
}

void BooleanExpression::RecalculateString() {
  string_ = expression_->str();
}

bool BooleanExpression::calc() {
  std::map<unsigned int, bool> vars;
  calculator::Context ctx{
      vars, std::cin, std::cout
  };
  return expression_->calc(ctx);
}

bool BooleanExpression::calc(calculator::Context &ctx) {
  return expression_->calc(ctx);
}

void BooleanExpression::truthTable() {
  auto ctx = calculator::FakeContext(expression_);
  // Table-printer constants
  const uint32_t extend_by = 2;
  const uint32_t expression_length = string_.size() + extend_by;
  uint32_t variables_length = 0;
  for (auto a : ctx.vars)
    if (variables_length < ((log2(a.first) - 1) / 8 + 1))
      variables_length = ((log2(a.first) - 1) / 8 + 1);
  variables_length += extend_by;

  if (truth_table_ == nullptr)
    GenerateTruthTable();

  // Print title
  for (auto a : ctx.vars)
    std::cout << std::setfill(' ') << std::right << std::setw(variables_length) << ("x" + std::to_string(a.first));
  std::cout << std::setfill(' ') << std::right << std::setw(expression_length) << string_ << std::endl;

  // Print values
  for (int i = 0; i < (1ULL << size_); ++i) {
    for (auto a : ctx.vars)
      std::cout << std::setfill(' ') << std::right << std::setw(variables_length) << a.second;
    std::cout << std::setfill(' ') << std::right << std::setw(expression_length) << truth_table_[i] << std::endl;
    IncrementVariables(ctx.vars);
  }
} // truthTable()

BooleanExpression BooleanExpression::cnf() {
  return BooleanExpression(BuildNormalForm(true).c_str());
} // cnf()

BooleanExpression BooleanExpression::dnf() {
  return BooleanExpression(BuildNormalForm(false).c_str());
} // dnf()

BooleanExpression BooleanExpression::zhegalkin() {
  auto ctx = calculator::FakeContext(expression_);
  if (zhegalkin_ == nullptr)
    GenerateZhegalkin();
  std::string res = "";
  for (int i = 0; i < (1ULL << size_); ++i) {
    if (zhegalkin_[i]) {
      auto tmp = i;
      for (auto a : ctx.vars) {
        if (tmp & 1ULL) {
          res += "x" + std::to_string(a.first) + " & ";
        }
        tmp >>= 1;
      }
      if (res != "")
        res.erase(res.end() - 3, res.end());
      else
        res = "1";
      res += " + ";
    }
  }
  res.erase(res.end() - 3, res.end());

  return BooleanExpression(res.c_str());
}

unsigned char BooleanExpression::Mask() {
  unsigned char mask = 0b11111;
  if (truth_table_ == nullptr)
    GenerateTruthTable();
  if (zhegalkin_ == nullptr)
    GenerateZhegalkin();

  if (truth_table_[0])
    mask = mask & (unsigned char) (0b11111101); // 0

  if (!truth_table_[(1ULL << size_) - 1])
    mask = mask & (unsigned char) (0b11111011); // 1

  for (int i = 1; i < (1ULL << size_); ++i) {
    if (truth_table_[i] < truth_table_[i - 1])
      mask = mask & (unsigned char) (0b11111110);
  }
  for (int i = 0; i < (1ULL << size_) / 2; ++i) {
    if (truth_table_[i] != truth_table_[(1ULL << size_) - i - 1]) {
      mask = mask & (unsigned char) (0b11101111);
    }
  }
  for (int i = 3; i < (1ULL << size_); ++i) {
    if (((i & (i - 1)) != 0) && zhegalkin_[i]) {
      mask = mask & (unsigned char) (0b11110111);
    }
  }
  return mask;
}

BooleanExpression::operator std::string() const {
  return string_;
}


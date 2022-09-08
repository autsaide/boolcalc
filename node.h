//
// Created by autsaide on 02/09/22.
//

#ifndef TASK5_CALCULATOR_AUTSAIDE_NODE_H
#define TASK5_CALCULATOR_AUTSAIDE_NODE_H

#include <string>
#include <map>
#include <iostream>

namespace calculator {

struct Context {
  std::map<unsigned int, bool> vars;
  std::istream &input;
  std::ostream &output;
};



class Node {
 public:
  virtual bool calc(Context &ctx) const = 0;
  virtual std::string str() const = 0;
  virtual Node *copy() const = 0;
  virtual void fill(std::map<unsigned int, bool> &vars) const = 0;

  virtual ~Node() = default;

};

class UnaryNode : public Node {
 protected:
  Node *left_;
 public:
  explicit UnaryNode(Node *node) : left_(node) {}
  ~UnaryNode() override {
    delete left_;
    left_ = nullptr;
  }
  void fill(std::map<unsigned int, bool> &vars) const override {
    left_->fill(vars);
  }
};

class BinaryNode : public Node {
 protected:
  Node *left_;
  Node *right_;
 public:
  explicit BinaryNode(Node *left_node, Node *right_node) : left_(left_node), right_(right_node) {}
  ~BinaryNode() override {
    delete left_;
    delete right_;
    left_ = nullptr;
    right_ = nullptr;
  }
  void fill(std::map<unsigned int, bool> &vars) const override {
    right_->fill(vars);
    left_->fill(vars);
  }

};

class NumberNode : public Node {
  const bool value_;
 public:
  explicit NumberNode(bool value) : value_(value) {};
  bool calc(Context &ctx) const override {
    return value_;
  }
  [[nodiscard]] std::string str() const override {
    return std::to_string(value_);
  }
  [[nodiscard]] Node *copy() const override {
    return new NumberNode(value_);
  }

  void fill(std::map<unsigned int, bool> &vars) const override {}
};

class VarNode : public Node {
  const unsigned int id_;
 public:
  explicit VarNode(unsigned int id) : id_(id) {};

  bool calc(Context &ctx) const override {
    auto cursor = ctx.vars.find(id_);
    if (cursor != ctx.vars.end())
      return cursor->second;
    bool tmp;
    ctx.output << "Variable x" << id_ << ": \t";
    ctx.input >> tmp;
    ctx.input.ignore();
    ctx.vars.insert({id_, tmp});
    return tmp;
  }

  [[nodiscard]] std::string str() const override {
    return "x" + std::to_string(id_);
  }
  [[nodiscard]] Node *copy() const override {
    return new VarNode(id_);
  }

  void fill(std::map<unsigned int, bool> &vars) const override {
    auto cursor = vars.find(id_);
    if (cursor == vars.end())
      vars.insert({id_, 0});
  }

};

class AndNode : public BinaryNode {
 public:
  explicit AndNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return right_->calc(ctx) && left_->calc(ctx);
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " & " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new AndNode(left_->copy(), right_->copy());
  }

};

class OrNode : public BinaryNode {
 public:
  explicit OrNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return right_->calc(ctx) || left_->calc(ctx);
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " v " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new OrNode(left_->copy(), right_->copy());
  }
};

class ImplNode : public BinaryNode {
 public:
  explicit ImplNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return !(right_->calc(ctx)) || (left_->calc(ctx));
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " > " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new ImplNode(left_->copy(), right_->copy());
  }
};

class RevImplNode : public BinaryNode {
 public:
  explicit RevImplNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return (right_->calc(ctx)) || !(left_->calc(ctx));
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " < " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new RevImplNode(left_->copy(), right_->copy());
  }
};

class XorNode : public BinaryNode {
 public:
  explicit XorNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return right_->calc(ctx) != left_->calc(ctx);
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " + " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new XorNode(left_->copy(), right_->copy());
  }
};

class EqNode : public BinaryNode {
 public:
  explicit EqNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return right_->calc(ctx) == left_->calc(ctx);
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " = " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new EqNode(left_->copy(), right_->copy());
  }
};

class ShefferNode : public BinaryNode {
 public:
  explicit ShefferNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return !(right_->calc(ctx)) || !(left_->calc(ctx));
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " | " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new ShefferNode(left_->copy(), right_->copy());
  }
};

class PierceNode : public BinaryNode {
 public:
  explicit PierceNode(Node *left_node, Node *right_node) : BinaryNode(left_node, right_node) {};

  bool calc(Context &ctx) const override {
    return !(right_->calc(ctx)) || !(left_->calc(ctx));
  }
  [[nodiscard]] std::string str() const override {
    return "(" + right_->str() + " ^ " + left_->str() + ")";
  }
  [[nodiscard]] Node *copy() const override {
    return new PierceNode(left_->copy(), right_->copy());
  }
};

class NegNode : public UnaryNode {
 public:
  explicit NegNode(Node *node) : UnaryNode(node) {};

  bool calc(Context &ctx) const override {
    return !(left_->calc(ctx));
  }

  [[nodiscard]] std::string str() const override {
    return "~" + left_->str();
  }
  [[nodiscard]] Node *copy() const override {
    return new NegNode(left_->copy());
  }

};

Context FakeContext(calculator::Node *expression);

} // calculator




#endif //TASK5_CALCULATOR_AUTSAIDE_NODE_H

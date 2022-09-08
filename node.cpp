//
// Created by autsaide on 02/09/22.
//

#include "node.h"

namespace calculator {
Context FakeContext(calculator::Node *expression) {
  calculator::Context ctx{
      {}, std::cin, std::cout
  };
  expression->fill(ctx.vars);
  return ctx;
}
} // calculator


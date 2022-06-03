#include <fstream>
#include "boolexpr.h"
#include "iostream"

int main(int argc, char* argv[]){
  std::cout << BooleanExpression::calc("1+1");
  return 0;
}
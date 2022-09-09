#include <fstream>
#include "boolexpr.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

int main(int argc, char *argv[]) {

  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i];
    std::cout << '\n';
  }

  std::cout << '\n';
  std::cout << '\n';

  bool help = false;
  switch (argc) {
    case 4:{
      switch (strlen(argv[1])) {
        case 5:{
          if (!strcmp(argv[1], "-calc")) {
            std::ifstream in(argv[2]);
            std::ofstream out(argv[3]);
            if (!in.is_open()){
              std::cout << "Unable to open file " << argv[2];
              return 1;
            }
            if (!out.is_open()){
              std::cout << "Unable to open file " << argv[3];
              return 1;
            }
            while (!in.eof()) {
              std::string line;
              std::getline(in, line);
              auto bool_expression = BooleanExpression(line.c_str());
              std::map<unsigned int, bool> vars;
              calculator::Context ctx{
                  vars, std::cin, std::cout
              };
              std::cout << "\n---- " << line << " ----\n";
              out << bool_expression.calc(ctx) << '\n';
            }
          } else {
            help = true;
          }
          break;
        }
        case 4:{
          if (!strcmp(argv[1], "-cnf")) {
            std::ifstream in(argv[2]);
            std::ofstream out(argv[3]);
            if (!in.is_open()){
              std::cout << "Unable to open file " << argv[2];
              return 1;
            }
            if (!out.is_open()){
              std::cout << "Unable to open file " << argv[3];
              return 1;
            }
            while (!in.eof()) {
              std::string line;
              std::getline(in, line);
              auto bool_expression = BooleanExpression(line.c_str());
              out << std::string(bool_expression.cnf())  << '\n';
            }
          } else if (!strcmp(argv[1], "-dnf")) {
            std::ifstream in(argv[2]);
            std::ofstream out(argv[3]);
            if (!in.is_open()){
              std::cout << "Unable to open file " << argv[2];
              return 1;
            }
            if (!out.is_open()){
              std::cout << "Unable to open file " << argv[3];
              return 1;
            }
            while (!in.eof()) {
              std::string line;
              std::getline(in, line);
              auto bool_expression = BooleanExpression(line.c_str());
              out << std::string(bool_expression.dnf()) << '\n';
            }
          } else {
            help = true;
          }
          break;
        }
        case 3:{
          if (!strcmp(argv[1], "-zh")) {
            std::ifstream in(argv[2]);
            std::ofstream out(argv[3]);
            if (!in.is_open()){
              std::cout << "Unable to open file " << argv[2];
              return 1;
            }
            if (!out.is_open()){
              std::cout << "Unable to open file " << argv[3];
              return 1;
            }
            while (!in.eof()) {
              std::string line;
              std::getline(in, line);
              auto bool_expression = BooleanExpression(line.c_str());
              out << std::string(bool_expression.zhegalkin()) << '\n';
            }
          } else {
            help = true;
          }
          break;
        }
        case 7:{
          if (strcmp(argv[1], "-isfull") == 0) {
            std::cout << "ISFULL" << std::endl;
            std::vector<BooleanExpression> expressions;
            std::fstream in(argv[2]);
            std::ofstream out(argv[3]);
            if (!in.is_open()){
              std::cout << "Unable to open file " << argv[2];
              return 1;
            }
            if (!out.is_open()){
              std::cout << "Unable to open file " << argv[3];
              return 1;
            }
            std::cout << "FILES VALID" << std::endl;

            while (!in.eof()) {
              std::string line;
              std::cout << "GET LINE" << std::endl;
              std::getline(in, line);

              std::cout << "LINE " << line << std::endl;
              expressions.push_back(BooleanExpression(line.c_str()));
            }

            std::cout << "START" << std::endl;
            out << (IsFull(expressions) ? "yes" : "no");
          } else {
            help = true;
          }
          break;
        }
        default:{
          help = true;
          break;
        }
      } // switch strlen
      break;
    }
    case 3:{
      if (!strcmp(argv[1], "-table")) {
        std::ifstream in(argv[2]);
        if (!in.is_open()){
          std::cout << "Unable to open file " << argv[2];
          return 1;
        }
        while (!in.eof()) {
          std::string line;
          std::getline(in, line);
          auto bool_expression = BooleanExpression(line.c_str());
          std::cout << "\n ---- " << line << " ----\n";
          bool_expression.truthTable();
        }
      } else {
        help = true;
      }
      break;
    }
    default:{
      help = true;
      break;
    }
  } // switch argc

  if (help) {
    std::cout <<
                 "Usage: " <<argv[0] << " [OPTION] SOURCE [DEST]\n"
                 "\n"
                 "Options:\n"
                 "  -calc   calculate boolean expressions from the file SOURCE and output results into the file DEST\n"
                 "  -cnf    construct conjunctive normal form for boolean expressions from the file SOURCE and output results into the file DEST\n"
                 "  -dnf    construct disjunctive normal form for boolean expressions from the file SOURCE and output results into the file DEST\n"
                 "  -zh     construct zhegalkin polynomial for boolean expressions from the file SOURCE and output results into the file DEST\n"
                 "  -table  output truth tables for boolean expressions from the file SOURCE into console\n"
                 "  -isfull check system of boolean expressions for the completeness and output \"yes\" if system is complete, \"no\" if incomplete\n"
                 "\nBuild time: " << __TIME__ << " " << __DATE__ << " \n"
        ;
  }

  return 0;










  //std::cout << std::string("Build\t[") + __DATE__ + "] \t (" + __TIME__ + ")\n";

  /*
  switch (argc) {
    case 4:

  }
  */
  /*
  std::vector<BooleanExpression> a;
  std::fstream file(argv[2]);

  while (!file.eof()) {
    std::string line;
    getline(file, line);
    a.push_back(BooleanExpression(line.c_str()));
  }
  std::cout << IsFull(a);
  */

  return 0;
}
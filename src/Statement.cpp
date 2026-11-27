#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.
void LetStatement::execute(VarState& state, Program& program) const {
  // std::cerr << expr->evaluate(state) << '\n';
  state.setValue(varName, expr->evaluate(state));
}

void PrintStatement::execute(VarState& state, Program& program) const {
  std::cout << expr->evaluate(state) << '\n';
}

void InputStatement::execute(VarState& state, Program& program) const {
  std::cout << "?\n";
  int value = 0;
  std::cin >> value;
  state.setValue(varName, value);
}

void GotoStatement::execute(VarState& state, Program& program) const {
  program.changePC(targetLine);
}

void IfStatement::execute(VarState& state, Program& program) const {
  bool if_is_right = 0;
  switch (op) {
    case '=':
      if_is_right = (leftExpr->evaluate(state) == rightExpr->evaluate(state));
      break;
    case '>':
      if_is_right = (leftExpr->evaluate(state) > rightExpr->evaluate(state));
      break;
    case '<':
      if_is_right = (leftExpr->evaluate(state) < rightExpr->evaluate(state));
      break;
  }
  if (if_is_right) {
    program.changePC(targetLine);
  }
}

void RemStatement::execute(VarState& state, Program& program) const {}

void EndStatement::execute(VarState& state, Program& program) const {
  program.programEnd();
}

LetStatement::~LetStatement() { delete expr; }

PrintStatement::~PrintStatement() { delete expr; }

InputStatement::~InputStatement() {}

GotoStatement::~GotoStatement() {}

IfStatement::~IfStatement() {
  delete leftExpr;
  delete rightExpr;
}

RemStatement::~RemStatement() {}

EndStatement::~EndStatement() {}
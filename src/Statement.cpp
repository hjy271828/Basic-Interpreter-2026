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
void LetStatement::execute(VarState& state, Program& program) const {}

void PrintStatement::execute(VarState& state, Program& program) const {}

void InputStatement::execute(VarState& state, Program& program) const {}

void GotoStatement::execute(VarState& state, Program& program) const {}

void IfStatement::execute(VarState& state, Program& program) const {}

void RemStatement::execute(VarState& state, Program& program) const {}

void EndStatement::execute(VarState& state, Program& program) const {}

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
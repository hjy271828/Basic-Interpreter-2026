// TODO: Imply interfaces declared in the Program.hpp.
#include "Program.hpp"

#include "Statement.hpp"

Program::Program() {
  programCounter_ = -1;
  programEnd_ = false;
}

void Program::addStmt(int line, Statement* stmt) { recorder_.add(line, stmt); }

void Program::removeStmt(int line) { recorder_.remove(line); }

void Program::run() {
  if (programEnd_ || recorder_.nextLine(-1) == -1) {
    programEnd();
    return;
  }

  programCounter_ = recorder_.nextLine(-1);
  while (programCounter_ != -1) {
    const Statement* cur_stmt = recorder_.get(programCounter_);
    programCounter_ = recorder_.nextLine(programCounter_);
    cur_stmt->execute(vars_, *this);
    if (programEnd_) {
      return;
    }
  }
  programEnd();
}

void Program::list() const { recorder_.printLines(); }

void Program::clear() {
  recorder_.clear();
  vars_.clear();
}

void Program::execute(Statement* stmt) {
  if (stmt != nullptr) {
    stmt->execute(vars_, *this);
  }
}

int Program::getPC() const noexcept { return programCounter_; }

void Program::changePC(int line) { programCounter_ = line; }

void Program::programEnd() {
  clear();
  programEnd_ = true;
  programCounter_ = -1;
}

void Program::resetAfterRun() noexcept {
  clear();
  programEnd_ = true;
  programCounter_ = -1;
}

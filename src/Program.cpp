// TODO: Imply interfaces declared in the Program.hpp.
#include "Program.hpp"

#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program() {
  programCounter_ = -1;
  programEnd_ = false;
}

void Program::addStmt(int line, Statement* stmt) {
  recorder_.add(line, stmt);
  programCounter_ = recorder_.nextLine(-1);
}

void Program::removeStmt(int line) {
  recorder_.remove(line);
  programCounter_ = recorder_.nextLine(-1);
}

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
      break;
    }
  }
  programEnd();
  resetAfterRun();
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

void Program::changePC(int line) {
  if (!recorder_.hasLine(line)) {
    throw BasicError("LINE NUMBER ERROR");
    return;
  }
  programCounter_ = line;
}

void Program::programEnd() {
  programEnd_ = true;
  programCounter_ = recorder_.nextLine(-1);
}

void Program::resetAfterRun() noexcept {
  programEnd_ = false;
  programCounter_ = recorder_.nextLine(-1);
}

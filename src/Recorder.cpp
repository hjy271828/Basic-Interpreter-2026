// TODO: Imply interfaces declared in the Recorder.hpp.
#include "Recorder.hpp"

#include <iostream>

Recorder::~Recorder() { clear(); }

void Recorder::add(int line, Statement* stmt) {
  for (auto& [cur_line, cur_stmt] : statements) {
    if (cur_line == line) {
      delete cur_stmt;
      cur_stmt = stmt;
      return;
    }
  }
  statements[line] = stmt;
}

void Recorder::remove(int line) {
  for (auto& [cur_line, cur_stmt] : statements) {
    if (cur_line == line) {
      delete cur_stmt;
      statements.erase(line);
      return;
    }
  }
}

const Statement* Recorder::get(int line) const noexcept {
  for (auto& [cur_line, cur_stmt] : statements) {
    if (cur_line == line) {
      return cur_stmt;
    }
  }
  return nullptr;
}

bool Recorder::hasLine(int line) const noexcept {
  for (auto& [cur_line, cur_stmt] : statements) {
    if (cur_line == line) {
      return true;
    }
  }
  return false;
}

void Recorder::clear() noexcept {
  for (auto& [cur_line, cur_stmt] : statements) {
    delete cur_stmt;
  }
  statements.clear();
}

void Recorder::printLines() const {
  std::string out;
  for (auto& [cur_line, cur_stmt] : statements) {
    out += cur_stmt->text() + '\n';
  }
  std::cout << out;
}

int Recorder::nextLine(int line) const noexcept {
  for (auto& [cur_line, cur_stmt] : statements) {
    if (cur_line > line) {
      return cur_line;
    }
  }
  return -1;
}
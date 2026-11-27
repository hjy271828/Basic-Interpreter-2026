#pragma once

#include <memory>
#include <shared_mutex>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.
class GotoStatement : public Statement {
 public:
  ~GotoStatement();
  void execute(VarState& state, Program& program) const;
  GotoStatement(std::string source, int targetLine)
      : Statement(source), targetLine(targetLine) {}

 private:
  int targetLine;
};

class LetStatement : public Statement {
 public:
  ~LetStatement();
  void execute(VarState& state, Program& program) const;
  LetStatement(std::string source, std::string varName, Expression* expr)
      : Statement(source), varName(varName), expr(expr) {};

 private:
  std::string varName;
  Expression* expr;
};

class IfStatement : public Statement {
 public:
  ~IfStatement();
  void execute(VarState& state, Program& program) const;
  IfStatement(std::string source, Expression* leftExpr, char op,
              Expression* rightExpr, int targetLine)
      : Statement(source),
        leftExpr(leftExpr),
        op(op),
        rightExpr(rightExpr),
        targetLine(targetLine) {}

 private:
  Expression* leftExpr;
  char op;
  Expression* rightExpr;
  int targetLine;
};

class RemStatement : public Statement {
 public:
  ~RemStatement();
  void execute(VarState& state, Program& program) const;
  RemStatement(std::string source) : Statement(source) {}
};
class EndStatement : public Statement {
 public:
  ~EndStatement();
  void execute(VarState& state, Program& program) const;
  EndStatement(std::string source) : Statement(source) {};
};

class PrintStatement : public Statement {
 public:
  ~PrintStatement();
  void execute(VarState& state, Program& program) const;
  PrintStatement(std::string source, Expression* expr)
      : Statement(source), expr(expr) {}

 private:
  Expression* expr;
};

class InputStatement : public Statement {
 public:
  ~InputStatement();
  void execute(VarState& state, Program& program) const;
  InputStatement(std::string source, std::string varName)
      : Statement(source), varName(varName) {}

 private:
  std::string varName;
};

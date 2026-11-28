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
  LetStatement(std::string source, std::string varName,
               std::unique_ptr<Expression> expr)
      : Statement(source), varName(varName), expr(std::move(expr)) {};

 private:
  std::string varName;
  std::unique_ptr<Expression> expr;
};

class IfStatement : public Statement {
 public:
  ~IfStatement();
  void execute(VarState& state, Program& program) const;
  IfStatement(std::string source, std::unique_ptr<Expression> leftExpr, char op,
              std::unique_ptr<Expression> rightExpr, int targetLine)
      : Statement(source),
        leftExpr(std::move(leftExpr)),
        op(op),
        rightExpr(std::move(rightExpr)),
        targetLine(targetLine) {}

 private:
  std::unique_ptr<Expression> leftExpr;
  char op;
  std::unique_ptr<Expression> rightExpr;
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
  PrintStatement(std::string source, std::unique_ptr<Expression> expr)
      : Statement(source), expr(std::move(expr)) {}

 private:
  std::unique_ptr<Expression> expr;
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

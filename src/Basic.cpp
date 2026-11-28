#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // TODO: The main function.
      TokenStream new_tokens(lexer.tokenize(line));
      if (new_tokens.empty()) {
        continue;
      }

      TokenType first_token_type = new_tokens.peek()->type;
      if (first_token_type == TokenType::RUN) {
        program.run();
        continue;
      } else if (first_token_type == TokenType::CLEAR) {
        program.clear();
        continue;
      } else if (first_token_type == TokenType::HELP) {
        std::cout
            << "支持的命令:\n  RUN - 开始执行程序\n  LIST - "
               "列出当前所有的程序行\n  CLEAR - 清除当前所有的程序行\n  QUIT - "
               "退出解释器\n  HELP - 打印帮助信息\n\n程序指令:\n  [行号] REM "
               "<注释> - 注释行\n  [行号] LET <变量> = <表达式> - 赋值语句\n  "
               "[行号] PRINT <表达式> - 打印表达式值\n  [行号] INPUT <变量> - "
               "输入变量值\n  [行号] END - 结束程序\n  [行号] GOTO <行号> - "
               "跳转到指定行\n  [行号] IF <表达式1> <操作符> <表达式2> THEN "
               "<行号> - 条件跳转\n  [行号] INDENT - 进入新的作用域块\n  "
               "[行号] DEDENT - 退出当前作用域块\n\n立即执行指令 (不带行号):\n "
               " LET <变量> = <表达式>\n  PRINT <表达式>\n  INPUT <变量>\n  "
               "INDENT\n  DEDENT\n";
        continue;
      } else if (first_token_type == TokenType::LIST) {
        program.list();
        continue;
      } else if (first_token_type == TokenType::QUIT) {
        program.programEnd();
        program.clear();
        break;
      }
      ParsedLine new_parsed_line = parser.parseLine(new_tokens, line);
      std::optional<int> cur_line = new_parsed_line.getLine();
      if (cur_line == std::nullopt) {
        std::shared_ptr<Statement> cur_stmt = new_parsed_line.getStatement();
        program.execute(cur_stmt);
      } else {
        std::shared_ptr<Statement> cur_stmt = new_parsed_line.fetchStatement();
        if (cur_stmt == nullptr) {
          program.removeStmt(cur_line.value());
        } else {
          program.addStmt(cur_line.value(), cur_stmt);
        }
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}
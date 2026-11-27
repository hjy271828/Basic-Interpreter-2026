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
      TokenStream new_tokens = lexer.tokenize(line);
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
            << "该项目从命令行读取对应的输入，一次读入一行，直到读入EOF。每读入"
               "一行后，程序会对该行进行解析并执行相应的命令。项目支持的命令包"
               "括：\n - "
               "一般指令：由行号和命令组成，行号为正整数，命令为以下几种之一："
               "\n    - `REM <comment>`：注释行，程序忽略该行内容。\n    - "
               "`LET "
               "<var> = <expr>`：将表达式的值赋给变量。\n    - `PRINT "
               "<expr>`：计算表达式的值并打印输出。\n    - `INPUT "
               "<var>`：输出一个 "
               "`?`，之后从标准输入读取一个整数并赋值给变量。\n    - "
               "`END`：程序结束执行。 \n    - `GOTO "
               "<line>`：跳转到指定行号执行。\n    - `IF <expr1> <op> <expr2> "
               "THEN <line>`：条件跳转，若 `expr1` 和 `expr2` 通过操作符 `op` "
               "比较为真，则跳转到指定行号执行。支持的操作符包括 `=`, `<`, "
               "`>`。\n  如果只有一个行号，代表删除对应指令。\n - "
               "立即执行指令：不带行号的命令，直接执行。支持的命令包括：`LET`"
               "，`PRINT` 与 `INPUT` 对应的指令。\n - "
               "解释器指令：用于驱动整个解释器而非需要被解释的指令。包括：\n   "
               " - `RUN`：开始执行程序，从最小行号的行开始。\n    - "
               "`LIST`：列出当前所有的程序行，按行号升序排列。\n    - "
               "`CLEAR`：清除当前所有的程序行。\n    - `QUIT`：退出解释器。"
               "\n    - `HELP`：打印帮助信息，列出所有支持的命令及其用法。\n";
        continue;
      } else if (first_token_type == TokenType::LIST) {
        program.list();
        continue;
      } else if (first_token_type == TokenType::QUIT) {
        program.programEnd();
        return 0;
      }
      ParsedLine new_parsed_line = parser.parseLine(new_tokens, line);
      std::optional<int> cur_line = new_parsed_line.getLine();
      Statement* cur_stmt = new_parsed_line.getStatement();
      if (cur_line == std::nullopt) {
        program.execute(cur_stmt);
      } else {
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
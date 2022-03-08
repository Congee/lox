#include <fmt/format.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "lex.hpp"
#include "token.hpp"

namespace fs = std::filesystem;

class Lox {
  bool had_error = false;

 public:
  void run(const std::string &source) {
    Lexer lexer{source};
    auto tokens = lexer.scan_tokens();
    if (tokens) {
      for (auto tok : tokens.value()) {
        std::puts(tok.to_string().c_str());
      }
    } else {
      fmt::print(stderr, tokens.error());
    }
  }

  void run_file(const fs::path &p) {
    std::ifstream in{p};
    std::ostringstream ss;
    ss << in.rdbuf();
    run(ss.str());
    if (had_error) exit(65);
  }

  void run_prompt() {
    for (std::string line; std::getline(std::cin, line);) {
      run(line);
      had_error = false;
    }
  }
};

int main(int argc, char *argv[]) {
  Lox lox{};
  if (argc > 2) {
    std::puts("Usage: jlox [script]");
    exit(64);
  } else if (argc == 2) {
    const fs::path p = argv[0];
    lox.run_file(p);
  } else {
    lox.run_prompt();
  }
}

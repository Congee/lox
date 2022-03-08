#pragma once

#include <cstdio>
#include <tl/expected.hpp>

#include "token.hpp"

using Error = const std::string;

class Lexer {
  const std::string source;
  std::vector<Token> tokens;

  uint32_t start = 0;
  uint32_t current = 0;
  uint32_t line = 0;

  char advance() { return source[current++]; }

  void add_token(TokenType type) { add_token(type, ""); }
  void add_token(TokenType type, const std::string &literal) {
    const auto text = source.substr(start, current);
    tokens.emplace_back(type, text, literal, line);
  }

  void error(int line, const std::string &msg) { report(line, "", msg); }

  void report(int line, const std::string &where, const std::string &msg) {
    // had_error = true;
    fmt::print(stderr, "[line {} ] Error {}: {}", line, where, msg);
    std::fflush(stderr);
    exit(128);
  }

  bool is_at_end() const { return current >= source.length(); }

  bool match(const char expected) {
    if (is_at_end()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
  }

  tl::expected<void, const std::string> scan_token() {
    char c = advance();
    switch (c) {
      case ')': add_token(RIGHT_PAREN); return {};
      case '{': add_token(LEFT_BRACE); return {};
      case '}': add_token(RIGHT_BRACE); return {};
      case ',': add_token(COMMA); return {};
      case '.': add_token(DOT); return {};
      case '-': add_token(MINUS); return {};
      case '+': add_token(PLUS); return {};
      case ';': add_token(SEMICOLON); return {};
      case '*': add_token(STAR); return {};
      case '!': add_token(match('=') ? BANG_EQUAL : BANG); return {};
      default:
        return tl::make_unexpected(
          fmt::format("Unexpected character \"{}\"", c));  // TODO: line_nr
    }
  };

 public:
  Lexer(const std::string &__source) : source(__source) {}

  tl::expected<const std::vector<Token>, Error> scan_tokens() {
    while (!is_at_end()) {
      start = current;
      const auto result = scan_token();
      if (!result) return tl::make_unexpected(result.error());
    }

    tokens.emplace_back(END, "", "", line);
    return {tokens};
  };
};

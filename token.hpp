#pragma once

#include <fmt/format.h>

#include <string>

enum TokenType {
  // single char
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // keywords
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TURE,
  VAR,
  WHILE,

  // eof
  END
};

class Token {
  const TokenType type;
  const std::string lexeme;
  const std::string literal;  // TODO
  const uint32_t line;

 public:
  Token(const TokenType __type, const std::string &__lexeme,
    const std::string &__literal, const uint32_t __line)
    : type(__type), lexeme(__lexeme), literal(__literal), line(__line) {}

  const std::string to_string() const {
    return fmt::format("{} {} {}", type, lexeme, literal);
  };
};

#pragma once

#include <cctype>
#include <cstdio>
#include <tl/expected.hpp>

#include "token.hpp"
#include "utils.hpp"

using Error = const std::string;

template <typename... Args>
auto err(Args... args) {
  return tl::make_unexpected(fmt::format(args...));
}

class Lexer {
  const std::string source;
  std::vector<Token> tokens;

  uint32_t start = 0;
  uint32_t current = 0;
  uint32_t line = 0;

  const std::unordered_map<std::string, Token::Type> keywords{
    {"and", Token::Type::AND},
    {"class", Token::Type::CLASS},
    {"else", Token::Type::ELSE},
    {"false", Token::Type::FALSE},
    {"for", Token::Type::FOR},
    {"fun", Token::Type::FUN},
    {"if", Token::Type::IF},
    {"nil", Token::Type::NIL},
    {"or", Token::Type::OR},
    {"print", Token::Type::PRINT},
    {"return", Token::Type::RETURN},
    {"super", Token::Type::SUPER},
    {"this", Token::Type::THIS},
    {"true", Token::Type::TRUE},
    {"var", Token::Type::VAR},
    {"while", Token::Type::WHILE},
  };

  char advance() { return source[current++]; }

  void add_token(Token::Type type) { add_token(type, ""); }
  void add_token(Token::Type type, const std::string &literal) {
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

    advance();
    return true;
  }

  char peek() const {
    if (is_at_end()) return '\0';
    return source[current];
  }

  tl::expected<void, const std::string> string() {
    while (peek() != '"' && !is_at_end()) {
      if (peek() == '\n') line++;
      advance();
    }
    if (is_at_end()) {
      // return tl::make_unexpected(fmt::format("{}: Unterminated string",
      // line));
      return err("{}: Unterminated string", line);
    }

    advance();

    add_token(Token::Type::STRING, source.substr(start + 1, current - 1));
    return {};
  }

  tl::expected<void, const std::string> number() {
    while (std::isdigit(peek())) advance();
    if (peek() == '.' && std::isdigit(peek_next())) {
      advance();
      while (std::isdigit(peek())) advance();
    }

    // TODO: double instead of string literal
    add_token(Token::Type::NUMBER, source.substr(start, current));
    return {};
  }

  char peek_next() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
  }

  tl::expected<void, const std::string> identifier() {
    while (std::isalnum(peek()) || peek() == '_')
      advance();  // XXX: upper case, underscore?
    const auto text = source.substr(start, current);
    const auto found = keywords.find(text);
    add_token(  // TODO
      found != keywords.end() ? Token::Type::IDENTIFIER : found->second);
    return {};
  }

  tl::expected<void, const std::string> scan_token() {
    char c = advance();
    switch (c) {
      case ')': add_token(Token::Type::RIGHT_PAREN); return {};
      case '{': add_token(Token::Type::LEFT_BRACE); return {};
      case '}': add_token(Token::Type::RIGHT_BRACE); return {};
      case ',': add_token(Token::Type::COMMA); return {};
      case '.': add_token(Token::Type::DOT); return {};
      case '-': add_token(Token::Type::MINUS); return {};
      case '+': add_token(Token::Type::PLUS); return {};
      case ';': add_token(Token::Type::SEMICOLON); return {};
      case '*': add_token(Token::Type::STAR); return {};
      case '!':
        add_token(match('=') ? Token::Type::BANG_EQUAL : Token::Type::BANG);
        return {};
      case '=':
        add_token(match('=') ? Token::Type::EQUAL_EQUAL : Token::Type::EQUAL);
        return {};
      case '<':
        add_token(match('=') ? Token::Type::LESS_EQUAL : Token::Type::LESS);
        return {};
      case '>':
        add_token(
          match('=') ? Token::Type::GREATER_EQUAL : Token::Type::GREATER);
        return {};
      case '/':
        if (match('/')) {
          while (peek() != '\n' && !is_at_end())
            advance();  // XXX: advance() more
        } else {
          add_token(Token::Type::SLASH);
        };
        return {};
      case ' ':
      case '\r':
      case '\t': return {};
      case '\n': line++; return {};
      case '"': return string();

      default:
        if (std::isdigit(c)) {
          return number();
        } else if (std::isalpha(c)) {
          return identifier();
        } else {
          return err("Unexpected character \"{}\"", c);  // TODO: line_nr
        }
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

    tokens.emplace_back(Token::Type::END, "", "", line);
    return {tokens};
  };
};

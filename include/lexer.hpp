#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "diagnostics.hpp"
namespace minic {
enum class TokenType { Identifier, Number, Plus, Minus, Star, Slash, LParen, RParen, Assign, Semicolon, EndOfFile };
const char* tokenTypeName(TokenType type);
struct Token { TokenType type = TokenType::EndOfFile; std::string lexeme; std::int64_t value = 0; SourceLocation loc; };
class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();
    const Diagnostics& diagnostics() const { return diagnostics_; }
private:
    bool atEnd() const; char peek() const; char advance(); void skipWhitespace();
    void error(SourceLocation loc, const std::string& message);
    Token lexIdentifier(SourceLocation start); bool lexNumber(SourceLocation start, Token& out);
    std::string source_; std::size_t pos_ = 0; int line_ = 1; int column_ = 1; Diagnostics diagnostics_;
};
}

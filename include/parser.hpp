#pragma once
#include <vector>
#include "ast.hpp"
#include "lexer.hpp"
namespace minic {
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::vector<Assignment> parseProgram();
    const Diagnostics& diagnostics() const { return diagnostics_; }
private:
    const Token& current() const; bool check(TokenType t) const; bool match(TokenType t); const Token& consume(TokenType t,const std::string& message);
    Assignment parseAssignment(); ExprPtr expression(); ExprPtr term(); ExprPtr unary(); ExprPtr primary(); void error(SourceLocation loc,const std::string& msg); void synchronize();
    const std::vector<Token>& tokens_; std::size_t pos_=0; Diagnostics diagnostics_;
};
}

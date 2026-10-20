#include "parser.hpp"
#include <utility>
namespace minic {
Parser::Parser(const std::vector<Token>& t):tokens_(t){}
const Token& Parser::current() const { return tokens_[pos_<tokens_.size()?pos_:tokens_.size()-1]; }
bool Parser::check(TokenType t) const{return current().type==t;}
bool Parser::match(TokenType t){if(!check(t))return false;++pos_;return true;}
const Token& Parser::consume(TokenType t,const std::string& m){if(check(t)){const Token& x=current();++pos_;return x;}error(current().loc,m);throw 1;}
void Parser::error(SourceLocation l,const std::string& m){diagnostics_.push_back({l,m});}
std::vector<Assignment> Parser::parseProgram(){std::vector<Assignment> result;while(!check(TokenType::EndOfFile)){try{result.push_back(parseAssignment());}catch(int){synchronize();}}return result;}
Assignment Parser::parseAssignment(){const Token& name=consume(TokenType::Identifier,"expected variable name at start of assignment");consume(TokenType::Assign,"expected '=' after variable name");auto value=expression();consume(TokenType::Semicolon,"expected ';' after expression");return {name.lexeme,std::move(value),name.loc};}
ExprPtr Parser::expression(){auto e=term();while(check(TokenType::Plus)||check(TokenType::Minus)){char op=current().type==TokenType::Plus?'+':'-';auto l=current().loc;++pos_;auto rhs=term();e=std::make_unique<BinaryExpr>(op,std::move(e),std::move(rhs),l);}return e;}
ExprPtr Parser::term(){auto e=unary();while(check(TokenType::Star)||check(TokenType::Slash)){char op=current().type==TokenType::Star?'*':'/';auto l=current().loc;++pos_;auto rhs=unary();e=std::make_unique<BinaryExpr>(op,std::move(e),std::move(rhs),l);}return e;}
ExprPtr Parser::unary(){if(match(TokenType::Minus)){auto l=tokens_[pos_-1].loc;return std::make_unique<UnaryExpr>('-',unary(),l);}if(match(TokenType::Plus)){return unary();}return primary();}
ExprPtr Parser::primary(){if(match(TokenType::Number)){const auto& t=tokens_[pos_-1];return std::make_unique<NumberExpr>(t.value,t.loc);}if(match(TokenType::Identifier)){const auto& t=tokens_[pos_-1];return std::make_unique<VariableExpr>(t.lexeme,t.loc);}if(match(TokenType::LParen)){auto e=expression();consume(TokenType::RParen,"expected ')' after expression");return e;}error(current().loc,"expected a number, variable, or parenthesized expression");throw 1;}
void Parser::synchronize(){if(!check(TokenType::EndOfFile)&&pos_<tokens_.size())++pos_;while(!check(TokenType::EndOfFile)){if(pos_>0&&tokens_[pos_-1].type==TokenType::Semicolon)return;if(check(TokenType::Semicolon)){++pos_;return;}++pos_;}}
}

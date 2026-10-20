#include "lexer.hpp"
#include <cctype>
#include <limits>
#include <utility>
namespace minic { namespace {
bool identStart(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool digit(char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; }
bool identPart(char c) { return identStart(c) || digit(c); }
std::string charDescription(char c) { unsigned char u=static_cast<unsigned char>(c); if(u>=0x20 && u<0x7f) return std::string("'")+c+"'"; const char* h="0123456789ABCDEF"; return std::string("byte 0x")+h[u>>4]+h[u&15]; }
}
const char* tokenTypeName(TokenType t) { switch(t) {
case TokenType::Identifier:return "Identifier"; case TokenType::Number:return "Number"; case TokenType::Plus:return "Plus"; case TokenType::Minus:return "Minus"; case TokenType::Star:return "Star"; case TokenType::Slash:return "Slash"; case TokenType::LParen:return "LParen"; case TokenType::RParen:return "RParen"; case TokenType::Assign:return "Assign"; case TokenType::Semicolon:return "Semicolon"; case TokenType::EndOfFile:return "EndOfFile"; } return "Unknown"; }
Lexer::Lexer(std::string s):source_(std::move(s)){}
bool Lexer::atEnd() const { return pos_>=source_.size(); }
char Lexer::peek() const { return atEnd()?'\0':source_[pos_]; }
char Lexer::advance() { char c=source_[pos_++]; if(c=='\n'){++line_;column_=1;} else ++column_; return c; }
void Lexer::skipWhitespace(){while(!atEnd()&&(peek()==' '||peek()=='\t'||peek()=='\r'||peek()=='\n'))advance();}
void Lexer::error(SourceLocation l,const std::string& m){diagnostics_.push_back({l,m});}
Token Lexer::lexIdentifier(SourceLocation s){Token t; t.type=TokenType::Identifier;t.loc=s;while(!atEnd()&&identPart(peek()))t.lexeme+=advance();return t;}
bool Lexer::lexNumber(SourceLocation s,Token& t){t.type=TokenType::Number;t.loc=s;constexpr auto max=std::numeric_limits<std::int64_t>::max();std::int64_t v=0;bool overflow=false;while(!atEnd()&&digit(peek())){char c=advance();t.lexeme+=c;int d=c-'0';if(!overflow){if(v>(max-d)/10)overflow=true;else v=v*10+d;}}if(!atEnd()&&identStart(peek())){while(!atEnd()&&identPart(peek()))t.lexeme+=advance();error(s,"malformed number '"+t.lexeme+"'");return false;}if(overflow){error(s,"integer literal '"+t.lexeme+"' is too large");return false;}t.value=v;return true;}
std::vector<Token> Lexer::tokenize(){std::vector<Token> out;while(true){skipWhitespace();if(atEnd())break;SourceLocation s{line_,column_};char c=peek();if(identStart(c)){out.push_back(lexIdentifier(s));continue;}if(digit(c)){Token t;if(lexNumber(s,t))out.push_back(std::move(t));continue;}TokenType ty;switch(c){case '+':ty=TokenType::Plus;break;case '-':ty=TokenType::Minus;break;case '*':ty=TokenType::Star;break;case '/':ty=TokenType::Slash;break;case '(':ty=TokenType::LParen;break;case ')':ty=TokenType::RParen;break;case '=':ty=TokenType::Assign;break;case ';':ty=TokenType::Semicolon;break;default:advance();error(s,"invalid character "+charDescription(c));continue;}Token t;t.type=ty;t.lexeme=std::string(1,advance());t.loc=s;out.push_back(std::move(t));}Token eof;eof.type=TokenType::EndOfFile;eof.loc={line_,column_};out.push_back(eof);return out;}
}

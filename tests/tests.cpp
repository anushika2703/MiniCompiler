#include <iostream>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "parser.hpp"
#include "ir.hpp"
#include "optimizer.hpp"
#include "codegen.hpp"
namespace { int failures=0, checks=0;
#define CHECK(x) do{++checks;if(!(x)){++failures;std::cerr<<"FAIL "<<__LINE__<<": "<<#x<<'\n';}}while(false)
std::vector<minic::IRInstruction> compile(const std::string& s){minic::Lexer l(s);auto t=l.tokenize();CHECK(l.diagnostics().empty());minic::Parser p(t);auto a=p.parseProgram();CHECK(p.diagnostics().empty());return minic::generateIR(a);}
void lexerTests(){minic::Lexer l("a=(b+c)*2;");auto t=l.tokenize();CHECK(l.diagnostics().empty());CHECK(t.size()==11);CHECK(t[0].type==minic::TokenType::Identifier);CHECK(t[8].value==2);minic::Lexer bad("x = 12abc @;");bad.tokenize();CHECK(bad.diagnostics().size()==2);}
void parserTests(){minic::Lexer l("a = b + c * 2;");auto t=l.tokenize();minic::Parser p(t);auto a=p.parseProgram();CHECK(p.diagnostics().empty());CHECK(a.size()==1);minic::Lexer bad("a = (2 + 3;");auto bt=bad.tokenize();minic::Parser bp(bt);bp.parseProgram();CHECK(!bp.diagnostics().empty());}
void irTests(){auto ir=compile("a = (b + c) * 2;");auto s=minic::irToString(ir);CHECK(s.find("t1 = b")!=std::string::npos);CHECK(s.find("t3 = t1 + t2")!=std::string::npos);CHECK(s.find("a = t5")!=std::string::npos);}
void optimizerTests(){auto ir=compile("x = 2 * 3 + 4;");auto opt=minic::optimizeConstants(ir);bool found=false;for(const auto& i:opt)if(i.op==minic::IROp::LoadConst&&i.value==10)found=true;CHECK(found);}
void codegenTests(){auto ir=compile("a = b + c;");auto out=minic::generateAssembly(ir);CHECK(out.find("ADD")!=std::string::npos);CHECK(out.find("STORE [a]")!=std::string::npos);CHECK(minic::generatePtxPreview(ir).find(".entry")!=std::string::npos);}
}
int main(){lexerTests();parserTests();irTests();optimizerTests();codegenTests();std::cout<<checks<<" checks, "<<failures<<" failures\n";return failures?1:0;}

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "lexer.hpp"
#include "parser.hpp"
#include "ir.hpp"
#include "optimizer.hpp"
#include "codegen.hpp"
namespace { void printDiagnostics(const minic::Diagnostics& ds){for(const auto& d:ds)std::cerr<<d.format()<<'\n';} }
int main(int argc,char** argv){if(argc<2||argc>3){std::cerr<<"Usage: minicompiler <source-file> [--tokens|--ast|--ir|--asm|--ptx-preview]\n";return 2;}std::ifstream f(argv[1],std::ios::binary);if(!f){std::cerr<<"error: cannot open '"<<argv[1]<<"'\n";return 2;}std::stringstream buffer;buffer<<f.rdbuf();minic::Lexer lexer(buffer.str());auto tokens=lexer.tokenize();if(argc==3&&std::string(argv[2])=="--tokens"){for(const auto& t:tokens){std::cout<<t.loc.line<<':'<<t.loc.column<<"\t"<<minic::tokenTypeName(t.type);if(!t.lexeme.empty())std::cout<<"\t"<<t.lexeme;std::cout<<'\n';}printDiagnostics(lexer.diagnostics());return lexer.diagnostics().empty()?0:1;}if(!lexer.diagnostics().empty()){printDiagnostics(lexer.diagnostics());return 1;}minic::Parser parser(tokens);auto program=parser.parseProgram();if(!parser.diagnostics().empty()){printDiagnostics(parser.diagnostics());return 1;}auto ir=minic::generateIR(program);std::string mode=argc==3?argv[2]:"--asm";if(mode=="--ast"){std::cout<<"AST parsed successfully (use --ir to inspect generated representation).\n";return 0;}if(mode=="--ir"){std::cout<<minic::irToString(ir);return 0;}if(mode=="--asm"){auto optimized=minic::optimizeConstants(ir);std::cout<<minic::generateAssembly(optimized);return 0;}if(mode=="--ptx-preview"){std::cout<<minic::generatePtxPreview(ir);return 0;}std::cerr<<"Unknown option: "<<mode<<"\n";return 2;}

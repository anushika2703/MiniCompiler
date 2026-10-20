#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "diagnostics.hpp"
namespace minic {
struct Expr { virtual ~Expr() = default; SourceLocation loc; };
using ExprPtr = std::unique_ptr<Expr>;
struct NumberExpr final : Expr { explicit NumberExpr(std::int64_t v, SourceLocation l) : value(v) { loc=l; } std::int64_t value; };
struct VariableExpr final : Expr { explicit VariableExpr(std::string n, SourceLocation l) : name(std::move(n)) { loc=l; } std::string name; };
struct UnaryExpr final : Expr { UnaryExpr(char o, ExprPtr e, SourceLocation l) : op(o), operand(std::move(e)) { loc=l; } char op; ExprPtr operand; };
struct BinaryExpr final : Expr { BinaryExpr(char o, ExprPtr a, ExprPtr b, SourceLocation l) : op(o), left(std::move(a)), right(std::move(b)) { loc=l; } char op; ExprPtr left, right; };
struct Assignment { std::string name; ExprPtr value; SourceLocation loc; };
}

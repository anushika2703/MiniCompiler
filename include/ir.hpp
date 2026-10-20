#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "ast.hpp"
namespace minic {
enum class IROp { LoadConst, LoadVar, Add, Sub, Mul, Div, Neg, Store };
struct IRInstruction { IROp op; std::string dest; std::string left; std::string right; std::int64_t value=0; SourceLocation loc; };
std::vector<IRInstruction> generateIR(const std::vector<Assignment>& program);
std::string irToString(const std::vector<IRInstruction>& ir);
const char* irOpName(IROp op);
}

#pragma once
#include <vector>
#include "ir.hpp"
namespace minic {
// Folds constant arithmetic where safe. Division by zero remains an explicit operation.
std::vector<IRInstruction> optimizeConstants(const std::vector<IRInstruction>& ir);
}

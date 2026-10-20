#pragma once
#include <string>
#include <vector>
#include "ir.hpp"
namespace minic {
std::string generateAssembly(const std::vector<IRInstruction>& ir);
// Emits a conservative PTX-like subset for the supported operations. This output is not guaranteed valid PTX.
std::string generatePtxPreview(const std::vector<IRInstruction>& ir);
}

#pragma once
#include <string>
#include <vector>
namespace minic {
struct SourceLocation { int line = 1; int column = 1; };
struct Diagnostic {
    SourceLocation loc;
    std::string message;
    std::string format() const {
        return "error: " + std::to_string(loc.line) + ":" + std::to_string(loc.column) + ": " + message;
    }
};
using Diagnostics = std::vector<Diagnostic>;
}

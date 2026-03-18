#pragma once

#include "error/error.hpp"

#include <iosfwd>
#include <string>

void PrintDiagnostic(const Diagnostic& diagnostic, const std::string& filename);
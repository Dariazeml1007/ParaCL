
#pragma once
#include "error/error.hpp"
#include <string>
#include <vector>

class HintGenerator
{
public:
    static void Enhance(Diagnostic& diagnostic,
                        const std::vector<std::string>& source_lines);

private:
    static void AddSyntaxHints(Diagnostic& d,
                               const std::vector<std::string>& source_lines);
    static void AddLexicalHints(Diagnostic& d);
    static void AddSemanticHints(Diagnostic& d);
    static void AddRuntimeHints(Diagnostic& d);

    static void
    CheckPreviousLineForSemicolon(Diagnostic& d,
                                  const std::vector<std::string>& source_lines);
};

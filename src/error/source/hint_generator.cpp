#include "error/hint_generator.hpp"

void HintGenerator::Enhance(Diagnostic& diagnostic,
                            const std::vector<std::string>& source_lines)
{

    if (!diagnostic.add_message.empty())
        return;

    CheckPreviousLineForSemicolon(diagnostic, source_lines);

    if (diagnostic.add_message.empty())
    {
        switch (diagnostic.kind)
        {
        case DiagnosticKind::Syntax:
            AddSyntaxHints(diagnostic, source_lines);
            break;
        case DiagnosticKind::Lexical:
            AddLexicalHints(diagnostic);
            break;
        case DiagnosticKind::Semantic:
            AddSemanticHints(diagnostic);
            break;
        case DiagnosticKind::Runtime:
            AddRuntimeHints(diagnostic);
            break;
        default:
            break;
        }
    }
}

void HintGenerator::CheckPreviousLineForSemicolon(
    Diagnostic& d, const std::vector<std::string>& source_lines)
{
    if (!d.range.has_value())
        return;
    if (d.range->begin.line <= 1)
        return;
    if (d.range->begin.column > 2)
        return;

    int prev_idx = d.range->begin.line - 2;
    if (prev_idx < 0 || prev_idx >= static_cast<int>(source_lines.size()))
        return;

    std::string_view prev = source_lines[prev_idx];

    size_t end = prev.find_last_not_of(" \t\r\n");
    if (end == std::string_view::npos)
        return;

    std::string_view trimmed = prev.substr(0, end + 1);
    char last = trimmed.back();
    if (last != ';' && last != '{' && last != '}')
    {
        d.add_message.push_back(
            "possible missing ';' at the end of the previous statement");
    }
}

void HintGenerator::AddSyntaxHints(Diagnostic& d,
                                   const std::vector<std::string>& source_lines)
{
    (void)source_lines;

    std::string_view msg = d.message;

    if (msg.find("expecting '('") != std::string_view::npos)
    {
        if (msg.find("WHILE") != std::string_view::npos)
            d.add_message.push_back(
                "while condition must be in parentheses: while (expr) { }");
        else if (msg.find("IF") != std::string_view::npos)
            d.add_message.push_back(
                "if condition must be in parentheses: if (expr) { }");
    }

    if (msg.find("unexpected '}'") != std::string_view::npos)
        d.add_message.push_back("check for unmatched opening brace '{'");
}

void HintGenerator::AddLexicalHints(Diagnostic& d)
{
    if (d.message.find("unexpected character") != std::string::npos)
        d.add_message.push_back(
            "remove this character or replace it with a valid token");

    if (d.message.find("out of range") != std::string::npos)
        d.add_message.push_back("use a smaller integer or larger value");
}

void HintGenerator::AddSemanticHints(Diagnostic& d)
{
    if (d.message.find("undefined variable") != std::string::npos)
        d.add_message.push_back(
            "make sure the variable is declared before use");

    if (d.message.find("assignment in condition") != std::string::npos)
        d.add_message.push_back("use '==' for comparison instead of '='");
}

void HintGenerator::AddRuntimeHints(Diagnostic& d)
{
    if (d.message.find("division by zero") != std::string::npos)
        d.add_message.push_back("check that the divisor is not zero");
}

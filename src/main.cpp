#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ast/ast.hpp"
#include "ast/node.hpp"
#include "error/error.hpp"
#include "error/error_print.hpp"
#include "error/hint_generator.hpp"
#include "interpreter/interpreter.hpp"
#include "parser.tab.hpp"
#include "visitor/semantic_visitor.hpp"

extern FILE* yyin;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <source_file>\n";
        return 0;
    }

    std::vector<std::string> source_lines;
    std::ifstream file(argv[1]);
    std::string line;
    while (std::getline(file, line))
    {
        source_lines.push_back(line);
    }
    file.close();

    yyin = std::fopen(argv[1], "r");
    if (!yyin)
    {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return 1;
    }

    try
    {
        language::AST ast;
        yy::parser parser(&ast);

        if (parser.parse() != 0)
        {
            std::fclose(yyin);
            return 1;
        }

        std::fclose(yyin);
        yyin = nullptr;

        SemanticVisitor visitor;

        visitor.SetSourceLines(source_lines);

        if (ast.get_root())
        {
            visitor.analyze(ast.get_root());

            const auto& errors = visitor.get_errors();
            if (!errors.empty())
            {
                for (const auto& error : errors)
                {
                    Diagnostic diag = error.diagnostic();

                    HintGenerator::Enhance(diag, source_lines);

                    PrintDiagnostic(diag, argv[1]);
                }
                return 1;
            }
        }

        language::Interpreter interp;
        interp.Run(*ast.get_root());
        return 0;
    }
    catch (const DiagnosticError& e)
    {
        if (yyin)
        {
            std::fclose(yyin);
            yyin = nullptr;
        }

        Diagnostic diag = e.diagnostic();

        HintGenerator::Enhance(diag, source_lines);

        PrintDiagnostic(diag, argv[1]);
        return 1;
    }
    catch (const std::exception& e)
    {
        if (yyin)
        {
            std::fclose(yyin);
            yyin = nullptr;
        }

        Diagnostic diagnostic;
        diagnostic.kind = DiagnosticKind::Internal;
        diagnostic.message = e.what();

        HintGenerator::Enhance(diagnostic, source_lines);

        PrintDiagnostic(diagnostic, argv[1]);
        return 1;
    }
}

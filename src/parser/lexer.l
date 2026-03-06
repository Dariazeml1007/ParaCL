%option noyywrap
%option yylineno
%option nounput
%option noinput

%{
#include "parser.tab.hpp"
#include <string>
#include <iostream>

extern yy::parser::semantic_type* yylval;
extern yy::parser::location_type* yylloc;
extern language::AST* ast_ptr;

namespace yy {
    int yycolno = 1;
    std::string current_line;
    std::string last_complete_line;
}

#define SET_LOC_BEGIN yylloc->begin.line = yylineno; yylloc->begin.column = yy::yycolno
#define SET_LOC_END yylloc->end.line = yylineno; yylloc->end.column = yy::yycolno + yyleng - 1
#define UPDATE_COLUMN yy::yycolno += yyleng; yy::current_line.append(yytext, yyleng)
%}

%%

[0-9]+ {
    SET_LOC_BEGIN;
    UPDATE_COLUMN;
    SET_LOC_END;
    yylval->emplace<int>(std::stoi(yytext));
    return yy::parser::token::NUMBER;
}

"while"   { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::WHILE; }
"if"      { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::IF;    }
"print"   { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::PRINT; }
"?"       { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::SCANF; }
"else"    { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::ELSE;  }

[a-zA-Z_][a-zA-Z0-9_]* {
    SET_LOC_BEGIN;
    UPDATE_COLUMN;
    SET_LOC_END;
    yylval->emplace<std::string*>(ast_ptr->make_string(yytext));
    return yy::parser::token::VAR;
}

[ \t]+ {
    SET_LOC_BEGIN;
    UPDATE_COLUMN;
    SET_LOC_END;
}
"\r\n" {
    SET_LOC_BEGIN;
    yy::last_complete_line = yy::current_line;
    yy::current_line.clear();
    yy::yycolno = 1;
    SET_LOC_END;
}

"\n" {
    SET_LOC_BEGIN;
    yy::last_complete_line = yy::current_line;
    yy::current_line.clear();
    yy::yycolno = 1;
    SET_LOC_END;
}

"\r" {
    SET_LOC_BEGIN;
    yy::last_complete_line = yy::current_line;
    yy::current_line.clear();
    yy::yycolno = 1;
    SET_LOC_END;
}
"//"[^\n]* {
    SET_LOC_BEGIN;
    UPDATE_COLUMN;
    SET_LOC_END;
}

"+"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::PLUS;  }
"-"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::MINUS; }
"*"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::MUL;   }
"/"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::DIV;   }
"%"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::MOD;   }

"==" { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::EQ;    }
"!=" { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::NE;    }
"<=" { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::LE;    }
">=" { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::GE;    }
"<"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::L;     }
">"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::G;     }

"&&" { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::AND;   }
"||" { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::OR;    }
"!"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::NOT;   }

"("  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return '('; }
")"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return ')'; }
"{"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return '{'; }
"}"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return '}'; }
"="  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return yy::parser::token::EQUALS; }
";"  { SET_LOC_BEGIN; UPDATE_COLUMN; SET_LOC_END; return ';'; }

. {
    SET_LOC_BEGIN;
    UPDATE_COLUMN;
    SET_LOC_END;
    std::cerr << "Lexer error: unexpected character '" << yytext[0]
              << "' at line " << yylineno << ", column " << yy::yycolno - 1 << std::endl;
    return yytext[0];
}

%%

yy::parser::semantic_type* yylval;
yy::parser::location_type* yylloc;
language::AST* ast_ptr = nullptr;

int yylex(yy::parser::semantic_type* yylval_param,
          yy::parser::location_type* yylloc_param,
          language::AST* ast_param) {
    yylval = yylval_param;
    yylloc = yylloc_param;
    ast_ptr = ast_param;

    yylloc->begin.line = yylineno;
    yylloc->begin.column = yy::yycolno;

    return yylex();
}

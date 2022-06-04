/*****************************************************
 *  The GNU Flex Specification for Mind Language.
 *
 *  We have provided complete SECTION I & IV for you.
 *  Please complete SECTION II & III.
 *
 *  In case you want some debug support, we provide a
 *  "diagnose()" function for you. All you need is to 
 *  call this function in main.cpp.
 *
 *  Please refer to the ``GNU Flex Manual'' if you have
 *  problems about how to write the lexical rules.
 *
 *  Keltin Leung 
%option yylineno noyywrap nounistd nounput bison-locations never-interactive  noinput batch debug
 */
%option yylineno noyywrap nounput noinput batch

%option outfile="scanner.cpp"
/* %option outfile="scanner.cpp" header-file="scanner.hpp" */

/* SECTION I: preamble inclusion. */
%{
#include "config.hpp"
#include "ast/ast.hpp"
#include "parser.hpp"
using namespace mind;
#include "location.hpp"

#include <cstdlib>
#include <iostream>
#include <climits>

using namespace mind::err;
yy::location loc;
# define YY_DECL \
  yy::parser::symbol_type yylex ()
// ... and declare it for the parser's sake.
YY_DECL;

%}

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_ICONST (const std::string &s, const yy::parser::location_type& loc,const int a);
%}

/* SECTION II: macro definition */
NEWLINE       (\r|\n|\r\n)
WHITESPACE    ([ \t]+)
IDENTIFIER    ([A-Za-z][_0-9A-Za-z]*)

INTEGER       [1-9]([0-9]*)
HINTEGER     0[xX][0-9a-fA-F]+
DINTEGER     0[0-7]*


 // digit  [0-9]


/* SECTION III: matching rules (and actions) */
%x B C
%%
{WHITESPACE}  {       }
{NEWLINE}     { loc.lines (yyleng); loc.step (); }
"//"          { BEGIN(C);         }
<C>{NEWLINE}  { BEGIN(0); loc.lines (yyleng); loc.step ();        }
<C><EOF>      { return yy::parser::make_END (loc); }
<C>.          { /* ignore */      }

"/*"          { BEGIN(B); }
<B>"*/"       { BEGIN(0); }
<B>.|\n ;  
<B><EOF> { return yy::parser::make_END (loc); }
<<EOF>> { return yy::parser::make_END (loc); }

"void"         { return yy::parser::make_VOID  (loc);      }
"int"         { return yy::parser::make_INT  (loc);      }
"return"      { return yy::parser::make_RETURN  (loc);   }
";"          { return yy::parser::make_SEMICOLON  (loc); }    
"("          { return yy::parser::make_LPAREN  (loc);       }
")"          { return yy::parser::make_RPAREN  (loc);       }
"{"          { return yy::parser::make_LBRACE  (loc);       }
"}"          { return yy::parser::make_RBRACE  (loc);       }



"-"          { return yy::parser::make_MINUS  (loc);       }
"!"          { return yy::parser::make_LNOT  (loc);       }
"~"          { return yy::parser::make_BNOT  (loc);       }

"+"          { return yy::parser::make_PLUS  (loc);       }
"*"          { return yy::parser::make_TIMES(loc);       }
"/"          { return yy::parser::make_SLASH  (loc);       }
"%"          { return yy::parser::make_MOD  (loc);       }
"="          { return yy::parser::make_ASSIGN  (loc);       }

"if"         { return yy::parser::make_IF  (loc);       }
"else"         { return yy::parser::make_ELSE  (loc);       }
"while"       { return yy::parser::make_WHILE  (loc);       }
"break"       { return yy::parser::make_BREAK (loc);       }
"do"       { return yy::parser::make_DO (loc);       }
"continue"       { return yy::parser::make_CONTINUE (loc);       }
"for"       { return yy::parser::make_FOR (loc);       }

"<="       { return yy::parser::make_LEQ (loc);       }
">="       { return yy::parser::make_GEQ (loc);       }
"<"       { return yy::parser::make_LT (loc);       }
">"       { return yy::parser::make_GT (loc);       }
"=="       { return yy::parser::make_EQU (loc);       }
"!="       { return yy::parser::make_NEQ (loc);       }
"&&"       { return yy::parser::make_AND (loc);       }
"||"       { return yy::parser::make_OR (loc);       }
","       { return yy::parser::make_COMMA (loc);       }
"?"       { return yy::parser::make_QUESTION (loc);       }
":"       { return yy::parser::make_COLON (loc);       }
"const"   {return yy::parser::make_CONST (loc);      }

"["       { return yy::parser::make_LBRACK (loc);       }
"]"       { return yy::parser::make_RBRACK (loc);       }

{INTEGER}     {return make_ICONST(yytext, loc,10);}
{HINTEGER}     {return make_ICONST(yytext, loc,16);}
{DINTEGER}     {return make_ICONST(yytext, loc,8);}

{IDENTIFIER}  {return yy::parser::make_IDENTIFIER (yytext, loc); }

.             { } 

%%
/* SECTION IV: customized section */
int myline, mycol;
yy::parser::symbol_type
make_ICONST (const std::string &s, const yy::parser::location_type& loc,const int a)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, (int)a);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_ICONST ((int) n, loc);
}
void scan_begin(const char* filename){
  if (NULL == filename)
	yyin = stdin;
  else
	yyin = std::fopen(filename, "r");
}
void scan_end(){
   if (yyin != stdin)
	  std::fclose(yyin);
}
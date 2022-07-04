/*****************************************************
 *  The GNU Bison Specification for Mind Language.
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
 */
%output "parser.cpp"
%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%define api.token.constructor
%define parse.assert
%locations
/* SECTION I: preamble inclusion */
%code requires{
#include "config.hpp"
#include "ast/ast.hpp"
#include "location.hpp"
#include "parser.hpp"

using namespace mind;

void yyerror (char const *);
void setParseTree(ast::Program* tree);

  /* This macro is provided for your convenience. */
#define POS(pos)    (new Location(pos.begin.line, pos.begin.column))


void scan_begin(const char* filename);
void scan_end();
}
%code{
  #include "compiler.hpp"
}
/* SECTION II: definition & declaration */

/*   SUBSECTION 2.1: token(terminal) declaration */


%define api.token.prefix {TOK_}
%token
   VOID "void"
   END  0  "end of file"
   BOOL "bool"
   INT  "int"
   RETURN "return"
   CONST "const"
   IF "if"
   ELSE  "else"
   DO "do"
   WHILE "while"
   FOR "for"
   BREAK "break"
   CONTINUE "continue"
   EQU "=="
   NEQ "!="
   AND "&&" 
   OR "||"
   LEQ "<="
   GEQ ">="
   PLUS "+"
   MINUS "-"
   TIMES "*"
   SLASH "/"
   MOD "%"
   LT "<"
   GT ">"
   COLON ":"
   SEMICOLON ";"
   LNOT "!"
   BNOT "~"
   COMMA ","
   DOT "."
   ASSIGN "="
   QUESTION "?"
   LPAREN "("
   RPAREN ")"
   LBRACK "["
   RBRACK "]"
   LBRACE "{"
   RBRACE "}"
;
%token <std::string> IDENTIFIER "identifier"
%token<int> ICONST "iconst"
%nterm<mind::ast::StmtList*> StmtList
%nterm<mind::ast::VarList* > FormalList DouList 
%nterm<mind::ast::Program* > Program FoDList
%nterm<mind::ast::FuncDefn* > FuncDefn
%nterm<mind::ast::Type*> Type
%nterm<mind::ast::Statement*> Stmt  ReturnStmt ExprStmt IfStmt  CompStmt WhileStmt DeclStmt DeclStmt_tuo DeclStmt2 DoWhileStmt ForStmt
%nterm<mind::ast::Expr*> Expr LvalueExpr FExpr
%nterm<mind::ast::VarRef*> VarRef
%nterm<mind::ast::ExprList*> ExprList IndexExpr1
%nterm<mind::ast::DimList*> IndexExpr3 
%nterm<mind::ast::IndexExpr*> IndexExpr2
%nterm<mind::ast::InitVal*> InitVal InitVals
/*   SUBSECTION 2.2: associativeness & precedences */

%right    ASSIGN
%right   QUESTION
%left     OR
%left     AND
%left EQU NEQ
%left LEQ GEQ LT GT
%left     PLUS MINUS
%left     TIMES SLASH MOD
%right LNOT NEG BNOT FNEG
%nonassoc LBRACK DOT
/*https://en.cppreference.com/w/c/language/operator_precedence*/
%{
  /* we have to include scanner.hpp here... */
#define YY_NO_UNISTD_H 1
%}

/*   SUBSECTION 2.5: start symbol of the grammar */
%start Program

/* SECTION III: grammar rules (and actions) */
%%
Program     : FoDList
                { /* we don't write $$ = XXX here. */
				  setParseTree($1); }
            ;
FoDList :   
            FuncDefn 
                {$$ = new ast::Program($1,POS(@1)); } |
            FoDList FuncDefn{
                 {$1->func_and_globals->append($2);
                  $$ = $1; }
                }
            | DeclStmt
            {
                $$ = new ast::Program($1,POS(@1));
            }
            |FoDList DeclStmt
            {
                $1->func_and_globals->append($2);
                $$ = $1;
            }
            ;
FuncDefn : Type IDENTIFIER LPAREN FormalList RPAREN LBRACE StmtList RBRACE {
              $$ = new ast::FuncDefn($2,$1,$4,$7,POS(@1));
          } |
          Type IDENTIFIER LPAREN FormalList RPAREN SEMICOLON{
              $$ = new ast::FuncDefn($2,$1,$4,new ast::EmptyStmt(POS(@6)),POS(@1));
          } |
          VOID IDENTIFIER LPAREN FormalList RPAREN LBRACE StmtList RBRACE{
              $$ = new ast::FuncDefn($2,new ast::IntType(POS(@1)),$4,$7,POS(@1));
          } |
          VOID IDENTIFIER LPAREN FormalList RPAREN SEMICOLON{
              $$ = new ast::FuncDefn($2,new ast::IntType(POS(@1)),$4,new ast::EmptyStmt(POS(@6)),POS(@1));
          }
          ;

FormalList :  /* EMPTY */
                {$$ = new ast::VarList();} 

            |  Type IDENTIFIER FormalList
                { $3->append(new ast::VarDecl($2,$1,POS(@1)));
                  $$=$3;
                }
            |  COMMA Type IDENTIFIER  FormalList
                { $4->append(new ast::VarDecl($3,$2,POS(@1)));
                  $$=$4;
                }
            |  Type IDENTIFIER LBRACK RBRACK FormalList
                { $5->append(new ast::VarDecl(std::string("1"),$2,$1,POS(@1)));
                  $$=$5;
                }
            |  COMMA Type IDENTIFIER  LBRACK RBRACK FormalList
                { $6->append(new ast::VarDecl(std::string("1"),$3,$2,POS(@1)));
                  $$=$6;
                }
            ;



Type        : INT 
                { $$ = new ast::IntType(POS(@1)); }
StmtList    : /* empty */
                { $$ = new ast::StmtList(); }
            | StmtList Stmt
                { $1->append($2);
                  $$ = $1; }
            | StmtList DeclStmt
                { $1->append($2);
                  $$ = $1;
                }
            ;
Stmt        : ReturnStmt {$$ = $1;}|
              ExprStmt   {$$ = $1;}|
              IfStmt     {$$ = $1;}|
              WhileStmt  {$$ = $1;}|
              DoWhileStmt  {$$ = $1;}|
              ForStmt  {$$ = $1;}|
              CompStmt   {$$ = $1;}|
              BREAK SEMICOLON  
                {$$ = new ast::BreakStmt(POS(@1));} |
              CONTINUE SEMICOLON  
                {$$ = new ast::ContStmt(POS(@1));} |
              SEMICOLON
                {$$ = new ast::EmptyStmt(POS(@1));}
            ;

DeclStmt    : Type IDENTIFIER DouList SEMICOLON
                { $$ = new ast::VarDecl($2, $1, $3,POS(@1)); }
            | Type IDENTIFIER ASSIGN Expr DouList SEMICOLON 
                { $$ = new ast::VarDecl($2, $1, $4, $5,POS(@1)); }
            | Type IDENTIFIER IndexExpr2 DouList SEMICOLON
                { $$ = new ast::VarDecl($2, $1, $3,$4, POS(@1)); }
            | Type IDENTIFIER IndexExpr2 ASSIGN IndexExpr1 DouList SEMICOLON 
                { $$ = new ast::VarDecl($2, $1, $3,$5, $6,POS(@1)); }
            | CONST Type IDENTIFIER ASSIGN Expr DouList SEMICOLON 
                { $$ = new ast::VarDecl(std::string("const"),$3, $2, $5, $6,POS(@1)); }
            | CONST Type IDENTIFIER IndexExpr2 ASSIGN IndexExpr1 DouList SEMICOLON 
                { $$ = new ast::VarDecl(std::string("const"),$3, $2, $4,$6, $7,POS(@1)); }
            ;

IndexExpr3   : LBRACK ICONST RBRACK IndexExpr3 
                { $$ = $4;
                  $$->append_my($2);
                }
            | LBRACK ICONST RBRACK
                { $$ = new ast::DimList();
                  $$->append_my($2);
                }
            
            ;


IndexExpr1   : /* EMPTY */
                {$$ = new ast::ExprList();} 
            | COMMA Expr IndexExpr1
                { $$ = $3;
                  $$->append_my($2);
                }
            | COMMA IndexExpr1
                { $$ = $2;
                }
            | Expr IndexExpr1
                { $$ = $2;
                  $$->append_my($1);
                }
            | LBRACE IndexExpr1 RBRACE IndexExpr1 
                { 
                  $$ = $4;
                  $$->concate($2);
                }
            | LBRACE RBRACE
                { 
                  $$ = new ast::ExprList();
                  $$->append_my(new ast::IntConst(0, POS(@1)));
                }
            ;

InitVal:
	Expr								{$$ = new InitVal_EXP(POS(@1), InitVal_EXP); $$->son.push_back($1);}
	|LBRACE RBRACE						{$$ = new InitVal_NULL(POS(@1), InitVal_NULL);}
	|LBRACE InitVals RBRACE				{$$ = new Initval_(POS(@1), InitVal_); $$->son.push_back($2);}
;

InitVals:
	InitVal							{$$ = new Initvals_(POS(@1)); $$->son.push_back($1);}
	|InitVals COMMA InitVal			{$1->son.push_back($3);}
;

DouList     : /* EMPTY */
                {$$ = new ast::DouList();} 
            | COMMA IDENTIFIER DouList
                { $3->append(new ast::VarDecl($2,POS(@1)));
                  $$=$3;
                }
            | COMMA IDENTIFIER ASSIGN Expr DouList
                { $5->append(new ast::VarDecl($2, $4,POS(@1)));
                  $$=$5;
                }
            | COMMA IDENTIFIER IndexExpr2 DouList
                { $4->append(new ast::VarDecl($2, $3,POS(@1)));
                  $$=$4;
                }
            | COMMA IDENTIFIER IndexExpr2 ASSIGN IndexExpr1 DouList
                { $6->append(new ast::VarDecl($2, $3,$5,POS(@1)));
                  $$=$6;
                }
            ;
VarRef      : IDENTIFIER
                { $$ = new ast::VarRef($1, POS(@1)); }
            | IDENTIFIER IndexExpr2
                { $$ = new ast::VarRef($1, $2, POS(@1)); }
            ;


IndexExpr2   : LBRACK Expr RBRACK IndexExpr2
                { $$ = new ast::IndexExpr($2, $4->expr_list, POS(@1)); }
            | LBRACK Expr RBRACK
                { $$ = new ast::IndexExpr($2, new ast::ExprList(), POS(@1)); }

LvalueExpr  : VarRef
                { $$ = new ast::LvalueExpr($1, POS(@1)); }
            | VarRef ASSIGN Expr
                { $$ = new ast::AssignExpr($1, $3, POS(@2)); }
            ;

CompStmt    : LBRACE StmtList RBRACE
                {$$ = new ast::CompStmt($2,POS(@1));}
            ;
WhileStmt   : WHILE LPAREN Expr RPAREN Stmt
                { $$ = new ast::WhileStmt($3, $5, POS(@1)); }
            ;
DoWhileStmt : DO Stmt WHILE LPAREN Expr RPAREN 
                { $$ = new ast::DoWhileStmt($5, $2, POS(@1)); }
            ;
ForStmt     : FOR LPAREN FExpr SEMICOLON FExpr SEMICOLON FExpr RPAREN Stmt
                { $$ = new ast::ForStmt($3, $5,$7,$9, POS(@1)); }
            | FOR LPAREN DeclStmt FExpr SEMICOLON FExpr RPAREN Stmt
                { $$ = new ast::ForStmt($3, $4,$6,$8, POS(@1));}
            ;

FExpr       : /* empty */
                { $$=NULL ;}
            | Expr
                { $$=$1; }
            ;
IfStmt      : IF LPAREN Expr RPAREN Stmt
                { $$ = new ast::IfStmt($3, $5, new ast::EmptyStmt(POS(@5)), POS(@1)); }
            | IF LPAREN Expr RPAREN Stmt ELSE Stmt
                { $$ = new ast::IfStmt($3, $5, $7, POS(@1)); }
            ;

ReturnStmt  : RETURN Expr SEMICOLON
                { $$ = new ast::ReturnStmt($2, POS(@1)); }
            | RETURN SEMICOLON
                { $$ = new ast::ReturnStmt( POS(@1)); }
            ;
ExprStmt    : Expr SEMICOLON
                { $$ = new ast::ExprStmt($1, POS(@1)); } 
            ;         

ExprList    :  /* empty */
                { $$ = new ast::ExprList(); }
            |   Expr ExprList 
                { $2->append_my($1);
                  $$ = $2;
                }
            |   COMMA Expr ExprList
                { $3->append_my($2);
                  $$ = $3;
                }
            ;


Expr        : ICONST
                { $$ = new ast::IntConst($1, POS(@1)); }
            | IDENTIFIER LPAREN ExprList RPAREN
                { $$ = new ast::CallExpr($1, $3, POS(@1)); }         
            | LPAREN Expr RPAREN
                { $$ = $2; }
            | Expr QUESTION Expr COLON Expr %prec QUESTION
                { $$ = new ast::IfExpr($1, $3, $5, POS(@1)); }
            | LvalueExpr
                { $$ = $1; }
            
            | Expr PLUS Expr %prec PLUS
                { $$ = new ast::AddExpr($1, $3, POS(@2)); }
            | Expr MINUS Expr %prec MINUS
                { $$ = new ast::SubExpr($1, $3, POS(@2)); }
            | Expr TIMES Expr %prec TIMES
                { $$ = new ast::MulExpr($1, $3, POS(@2)); }
            | Expr SLASH Expr %prec SLASH
                { $$ = new ast::DivExpr($1, $3, POS(@2)); }
            | Expr MOD Expr %prec MOD
                { $$ = new ast::ModExpr($1, $3, POS(@2)); }
           
            | MINUS Expr  %prec NEG
                { $$ = new ast::NegExpr($2, POS(@1)); }

            | PLUS Expr %prec FNEG
                { $$ = new ast::FNegExpr($2, POS(@1)); }

            | LNOT Expr  %prec LNOT
                { $$ = new ast::BitNotExpr($2, POS(@1)); }
            | BNOT Expr  %prec BNOT
                { $$ = new ast::NotExpr($2, POS(@1)); }
            | Expr LEQ Expr %prec LEQ
                { $$ = new ast::LeqExpr($1,$3, POS(@2)); }
            | Expr GEQ Expr %prec GEQ
                { $$ = new ast::GeqExpr($1,$3, POS(@2)); }
            | Expr LT Expr %prec LT
                { $$ = new ast::LesExpr($1,$3, POS(@2)); }
            | Expr GT Expr %prec GT
                { $$ = new ast::GrtExpr($1,$3, POS(@2)); }
            | Expr EQU Expr %prec EQU
                { $$ = new ast::EquExpr($1,$3, POS(@2)); }
            | Expr NEQ Expr %prec NEQ
                { $$ = new ast::NeqExpr($1,$3, POS(@2)); }
            | Expr AND Expr %prec AND
                { $$ = new ast::AndExpr($1,$3, POS(@2)); }
            | Expr OR Expr %prec OR
                { $$ = new ast::OrExpr($1,$3, POS(@2)); }

            ;

%%

/* SECTION IV: customized section */
#include "compiler.hpp"
#include <cstdio>

static ast::Program* ptree = NULL;
extern int myline, mycol;   // defined in scanner.l

// bison will generate code to invoke me
void
yyerror (char const *msg) {
  err::issue(new Location(myline, mycol), new err::SyntaxError(msg));
  scan_end();
  std::exit(1);
}

// call me when the Program symbol is reduced
void
setParseTree(ast::Program* tree) {
  ptree = tree;
}

/* Parses a given mind source file.
 *
 * PARAMETERS:
 *   filename - name of the source file
 * RETURNS:
 *   the parse tree (in the form of abstract syntax tree)
 * NOTE:
 *   should any syntax error occur, this function would not return.
 */
ast::Program*
mind::MindCompiler::parseFile(const char* filename) {  
  scan_begin(filename);
  /* if (NULL == filename)
	yyin = stdin;
  else
	yyin = std::fopen(filename, "r"); */
  yy::parser parse;
  parse();
  scan_end();
  /* if (yyin != stdin)
	std::fclose(yyin); */
  
  return ptree;
}

void
yy::parser::error (const location_type& l, const std::string& m)
{
  //std::cerr << l << ": " << m << '\n';
  err::issue(new Location(l.begin.line, l.begin.column), new err::SyntaxError(m));
  
  scan_end();
  std::exit(1);
}

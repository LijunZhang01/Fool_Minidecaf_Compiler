/*****************************************************
 *  Implementation of "WhileStmt".
 *
 *  Please refer to ast/ast.hpp for the definition.
 *
 *  Keltin Leung 
 */

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;

/* Creates a new WhileStmt node.
 *
 * PARAMETERS:
 *   cond    - the test expression
 *   body    - the loop body
 *   l       - position in the source text
 */
ForStmt::ForStmt(Expr *cond1,Expr *cond2,Expr *cond3, Statement *loop_body, Location *l) {

    setBasicInfo(FOR_STMT, l);

    condition1=cond1;
    condition2=cond2;
    condition3=cond3;
    this->loop_body = loop_body;
    decl1=NULL;
}

ForStmt::ForStmt(Statement *cond1, Expr *cond2,Expr *cond3,Statement *loop_body, Location *l) {

    setBasicInfo(FOR_STMT, l);

    decl1=cond1;
    condition2=cond2;
    condition3=cond3;
    this->loop_body = loop_body;
    condition1=NULL;
}
/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void ForStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void ForStmt::dumpTo(std::ostream &os) {
    
}

/* Creates a new BreakStmt node.
 *
 * PARAMETERS:
 *   l       - position in the source text
 */


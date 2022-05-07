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
DoWhileStmt::DoWhileStmt(Expr *cond, Statement *body, Location *l) {

    setBasicInfo(DOWHILE_STMT, l);

    condition = cond;
    loop_body = body;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void DoWhileStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void DoWhileStmt::dumpTo(std::ostream &os) {
   
}


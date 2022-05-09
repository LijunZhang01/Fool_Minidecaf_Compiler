/*****************************************************
 *  Implementation of "AssignStmt".
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

/* Creates a new AssignExpr node.
 *
 * PARAMETERS:
 *   lv      - the left value
 *   e0      - the expresion on the right side (i.e. right value)
 *   l       - position in the source text
 */
CallExpr::CallExpr(std::string name, ExprList * e,Location *l) {

    setBasicInfo(CALL_EXPR, l);

    this->name=name;
    this->elist=e;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void CallExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void CallExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    //os << left << "=";

    newLine(os);
    //s << e << ")";
    decIndent(os);
}

/*****************************************************
 *  Implementation of "VarDecl".
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

/* Creates a new VarDecl node.
 *
 * PARAMETERS:
 *   n       - name of the variable
 *   t       - type of the variable
 *   l       - position in the source text
 */
VarDecl_2::VarDecl_2(Expr *v,Location *l) {

    setBasicInfo(VArDecl_2, l);

    val=v;
}



/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void VarDecl_2::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void VarDecl_2::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << val;
    newLine(os);
}

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
VarDecl_1::VarDecl_1(std::string n, Type *t, DeclList  *v_2,Location *l) {

    setBasicInfo(VArDecl_1, l);

    name = n;
    type = t;
    init = NULL;
    vardecl_2=v_2;
}

VarDecl_1::VarDecl_1(std::string n, Type *t, Expr *i, DeclList  *v_2,Location *l) {
    setBasicInfo(VArDecl_1, l);

    name = n;
    type = t;
    init = i;
    vardecl_2=v_2;
}

VarDecl_1::VarDecl_1(std::string n, Type *t, int d, Location *l) {

    setBasicInfo(VArDecl_1, l);

    name = n;
    type = t;
    init = NULL;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void VarDecl_1::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void VarDecl_1::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    if(vardecl_2==NULL){
        if (init == NULL) {
            os << " " << '"' << name << '"' << " " << type << ")";
        } else {
            os << " " << '"' << name << '"' << " " << type << "=";
            newLine(os);
            os << init << ")";
        }
        decIndent(os);
    }
    else{
        if (init == NULL) {
            os << " " << '"' << name << '"' << " " << type << ")";
            //os << vardecl_2;
        } else {
            os << " " << '"' << name << '"' << " " << type << "=";
            newLine(os);
            os << init << ")";
            //os << vardecl_2;
        }
        decIndent(os);
    }
}

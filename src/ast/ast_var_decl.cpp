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
Type *mind::quanju=NULL;
/* Creates a new VarDecl node.
 *
 * PARAMETERS:
 *   n       - name of the variable
 *   t       - type of the variable
 *   l       - position in the source text
 */
VarDecl::VarDecl(std::string n, Type *t, DouList *li,Location *l) {

    setBasicInfo(VAR_DECL, l);
    name = n;
    type = t;
    quanju = t;
    init = NULL;
    lian=li;
    if(lian!=NULL){
        for(ast::DouList::iterator it=lian->begin();
        it!=lian->end();++it){
            (*it)->type=t;
        }
    }
}

VarDecl::VarDecl(std::string n, Type *t, Expr *i, DouList *li,Location *l) {
    setBasicInfo(VAR_DECL, l);

    name = n;
    type = t;
    quanju=t;
    init = i;
    lian=li;
    if(lian!=NULL){
        for(ast::DouList::iterator it=lian->begin();
        it!=lian->end();++it){
            (*it)->type=t;
        }
    }
}

VarDecl::VarDecl(std::string n, Location *l) {

    setBasicInfo(VAR_DECL, l);
    name = n;
    type = quanju;
    init = NULL;
    lian=NULL;
}

VarDecl::VarDecl(std::string n, Expr *i, Location *l) {
    setBasicInfo(VAR_DECL, l);
    name = n;
    type = quanju;
    init = i;
    lian=NULL;
}

VarDecl::VarDecl(std::string n, Type *t, int d, Location *l) {

    setBasicInfo(VAR_DECL, l);

    name = n;
    type = t;
    init = NULL;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void VarDecl::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void VarDecl::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    if (init == NULL) {
        os << " " << '"' << name << '"' << " " << type << ")";
    } else {
        os << " " << '"' << name << '"' << " " << type << "=";
        newLine(os);
        os << init << ")";
    }
    decIndent(os);
}

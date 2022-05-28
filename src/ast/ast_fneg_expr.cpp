////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;



FNegExpr::FNegExpr(Expr *e0, Location *l) {

    setBasicInfo(ONEADD, l);

    e = e0;
}


void FNegExpr::accept(Visitor *v) { v->visit(this); }

void FNegExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << e << ")";
    decIndent(os);
}

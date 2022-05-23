
void Translation::visit(ast::AddExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    
    e->ATTR(value)=e->e1->ATTR(value)+e->e2->ATTR(value);
}

/* Translating an ast::SubExor node.
 */
void Translation::visit(ast::SubExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(value)=e->e1->ATTR(value)-e->e2->ATTR(value);
}


/* Translating an ast::MulExpr node.
 */
void Translation::visit(ast::MulExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(value)=e->e1->ATTR(value)*e->e2->ATTR(value);
}

/* Translating an ast::LesExpr node.
 */
void Translation::visit(ast::LesExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    if(e->e1->ATTR(value)<e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::GrtExpr node.
 */
void Translation::visit(ast::GrtExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    if(e->e1->ATTR(value)>e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::LeqExpr node.
 */
void Translation::visit(ast::LeqExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    if(e->e1->ATTR(value)<=e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::GeqExpr node.
 */
void Translation::visit(ast::GeqExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    if(e->e1->ATTR(value)>=e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}


/* Translating an ast::EquExpr node.
 */
void Translation::visit(ast::EquExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    if(e->e1->ATTR(value)==e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::NeqExpr node.
 */
void Translation::visit(ast::NeqExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    if(e->e1->ATTR(value)!=e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::AndExpr node.
 */
void Translation::visit(ast::AndExpr *e) {

    e->e1->accept(this);
    
    e->e2->accept(this);

    e->ATTR(value)=e->e1->ATTR(value)&&e->e2->ATTR(value);
}

/* Translating an ast::OrExpr node.
 */
void Translation::visit(ast::OrExpr *e) {
   
    e->e1->accept(this);
    
    e->e2->accept(this);
    
    e->ATTR(value)=e->e1->ATTR(value)||e->e2->ATTR(value);
}
/* Translating an ast::DivExpr node.
 */
void Translation::visit(ast::DivExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    
    e->ATTR(value)=e->e1->ATTR(value)/e->e2->ATTR(value);
}


/* Translating an ast::ModExpr node.
 */
void Translation::visit(ast::ModExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);


    e->ATTR(value)=e->e1->ATTR(value)%e->e2->ATTR(value);
}


/* Translating an ast::IntConst node.
 */
void Translation::visit(ast::IntConst *e) {
    e->ATTR(value)=e->value;
}

/* Translating an ast::NegExpr node.
 */
void Translation::visit(ast::NegExpr *e) {
    e->e->accept(this);

    e->ATTR(value)=-(e->e->ATTR(value));
}
/* Translating an ast::NotExpr node.
 */
void Translation::visit(ast::NotExpr *e) {
    e->e->accept(this);


    e->ATTR(value)=~(e->e->ATTR(value));
}
/* Translating an ast::BitNotExpr node.
 */
void Translation::visit(ast::BitNotExpr *e) {
    e->e->accept(this);


    e->ATTR(value)=!(e->e->ATTR(value));

}

/* Translating an ast::LvalueExpr node.
 *
 * NOTE:
 *   different Lvalue kinds need different translation
 */
void Translation::visit(ast::LvalueExpr *e) {
    // TODO
    e->lvalue->accept(this);
    switch (e->lvalue->getKind()) {
        case ast::ASTNode::VAR_REF:{
            ast::VarRef *ref = (ast::VarRef *)e->lvalue;
            // if(ref->ATTR(sym)->iscon){
            //     e->ATTR(val) = tr->genLoad(temp, 0);
            // }
            if(ref->ATTR(sym)->iscon){
                    e->ATTR(value) = ref->ATTR(sym)->con_val;
            }
            else{
                mind_assert(false);
            }
            
            break;
        }
        default:
            mind_assert(false);
    }
}



void Translation::visit(ast::VarRef *ref) {
    switch (ref->ATTR(lv_kind)) {
    case ast::Lvalue::SIMPLE_VAR:
        // nothing to do
        break;
    case ast::Lvalue::ARRAY_ELE:
        ref->ldim->accept(this);
        break;
    default:
        mind_assert(false); // impossible
    }
    // actually it is so simple :-)
}


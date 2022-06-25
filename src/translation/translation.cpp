/*****************************************************
 *  Implementation of the third translation pass.
 *
 *  In the third pass, we will: 
 * 
 *    translate all the statements and expressions
 *
 *  Keltin Leung 
 */

#include "translation.hpp"
#include "asm/offset_counter.hpp"
#include "ast/ast.hpp"
#include "compiler.hpp"
#include "config.hpp"
#include "scope/scope.hpp"
#include "symb/symbol.hpp"
#include "tac/tac.hpp"
#include "tac/trans_helper.hpp"
#include "type/type.hpp"
#include "scope/scope_stack.hpp"

using namespace mind;
using namespace mind::symb;
using namespace mind::tac;
using namespace mind::type;
using namespace mind::assembly;

/* Constructor.
 *
 * PARAMETERS:
 *   helper - the translation helper
 */
Translation::Translation(tac::TransHelper *helper) {
    mind_assert(NULL != helper);

    tr = helper;
}

/* Translating an ast::Program node.
 */
void Translation::visit(ast::Program *p) {
    if(!mind::aa.empty()){
        // std::cout<<mind::aa.size();
        // if(*(aa.begin())!=NULL){
        //     std::cout<<"Dfs";
        // }
            // std::cout<<(*(aa.begin()))->getName();
        for(auto it:mind::aa){
            it->attachEntryLabel(tr->getNewEntryLabel(it));
        }
        // for(auto it=mind::aa.begin();it!=mind::aa.end();it++){
        //     (*it)->attachEntryLabel(tr->getNewEntryLabel((*it)));
        // }
        // std::cout<<"fd";
    }
    
    for (auto it = p->func_and_globals->begin();
         it != p->func_and_globals->end(); ++it)
        (*it)->accept(this);
    
        // Function *v = (Function *)scopes->lookup("getint", p->getLocation(),false);
        // if(v!=NULL){
        //     v->attachEntryLabel(tr->getNewEntryLabel(v));
        // }
    
}

// three sugars for parameter of fset management
#define RESET_OFFSET() tr->getOffsetCounter()->reset(OffsetCounter::PARAMETER)
#define NEXT_OFFSET(x) tr->getOffsetCounter()->next(OffsetCounter::PARAMETER, x)

/* Translating an ast::FuncDefn node.
 *
 * NOTE:
 *   call tr->startFunc() before translating the statements and
 *   call tr->endFunc() after all the statements have been translated
 */
void Translation::visit(ast::FuncDefn *f) {
    Function *fun = f->ATTR(sym);

    // attaching function entry label
    
    fun->attachEntryLabel(tr->getNewEntryLabel(fun));
    
    // arguments
    int order = 0;
    for (auto it = f->formals->begin(); it != f->formals->end(); ++it) {
        auto v = (*it)->ATTR(sym);
        v->setOrder(order++);
        v->attachTemp(tr->getNewTempI4());
    }

    fun->offset = fun->getOrder() * POINTER_SIZE;

    RESET_OFFSET();

    // the other formal arguments
    for (ast::VarList::iterator it = f->formals->begin();
         it != f->formals->end(); ++it) {
        Variable *v = (*it)->ATTR(sym);
        v->offset = NEXT_OFFSET(v->getTemp()->size);
    }

    tr->startFunc(fun);

    // translates statement by statement
    for (auto it = f->stmts->begin(); it != f->stmts->end(); ++it)
        (*it)->accept(this);

    tr->genReturn(tr->genLoadImm4(0)); // Return 0 by default

    tr->endFunc();
}

/* Translating an ast::AssignStmt node.
 *
 * NOTE:
 *   different kinds of Lvalue require different translation
 */
void Translation::visit(ast::AssignExpr *s) {
    // TODO
    s->left->accept(this);
    s->e->accept(this);
    ast::VarRef *ref = (ast::VarRef *)(s->left);
    if(ref->ATTR(sym)->isGlobalVar()){
        Temp temp = tr->genLoadSymbol(ref->var);
        if(ref->ATTR(lv_kind) == ast::Lvalue::ARRAY_ELE)
            temp = tr->genAdd(temp, ref->ldim->ATTR(val));
        tr->genStore(s->e->ATTR(val), temp, 0);
        s->ATTR(val) = s->e->ATTR(val);
    }
    else{
        if(ref->ATTR(lv_kind) == ast::Lvalue::ARRAY_ELE){
            Temp temp = tr->genAdd(ref->ATTR(sym)->getTemp(), ref->ldim->ATTR(val));
            tr->genStore(s->e->ATTR(val), temp, 0);
            s->ATTR(val)=s->e->ATTR(val);
        }
        else{
            Temp temp = ((ast::VarRef *)(s->left))->ATTR(sym)->getTemp();
            tr->genAssign(temp, s->e->ATTR(val)); 
            s->ATTR(val) = temp;
        }
        
    }
    s->ATTR(value) = s->e->ATTR(value);
    ref->ATTR(sym)->value_v=s->e->ATTR(value);
}

/* Translating an ast::ExprStmt node.
 */
void Translation::visit(ast::ExprStmt *s) { s->e->accept(this); }

/* Translating an ast::IfStmt node.
 *
 * NOTE:
 *   you don't need to test whether the false_brch is empty
 */
void Translation::visit(ast::IfStmt *s) {
    //看一个具体示例就会发现简单明了，easy
    Label L1 = tr->getNewLabel(); // entry of the false branch
    Label L2 = tr->getNewLabel(); // exit
    s->condition->accept(this);
    //beqz搞到错误分支地址
    tr->genJumpOnZero(L1, s->condition->ATTR(val));
    //正确分支代码
    s->true_brch->accept(this);
    //jump跳到出口
    tr->genJump(L2); // done
    //genMarkLable:标记错误分支地址
    tr->genMarkLabel(L1);
    s->false_brch->accept(this);

    tr->genMarkLabel(L2);
}

void Translation::visit(ast::IfExpr *s) {
    // TODO
    Temp temp = tr->getNewTempI4();
    //看一个示例就会发现简单明了，easy
    Label L1 = tr->getNewLabel(); // entry of the false branch
    Label L2 = tr->getNewLabel(); // exit
    s->condition->accept(this);
    //beqz搞到错误分支地址
    tr->genJumpOnZero(L1, s->condition->ATTR(val));
    //正确分支代码
    s->true_brch->accept(this);
    //多加一步
    tr->genAssign(temp, s->true_brch->ATTR(val));
    //jump跳到出口
    tr->genJump(L2); // done
    //genMarkLable:标记错误分支地址
    tr->genMarkLabel(L1);
    s->false_brch->accept(this);
    tr->genAssign(temp, s->false_brch->ATTR(val));
    tr->genMarkLabel(L2);
    s->ATTR(val)=temp;
    
}
/* Translating an ast::WhileStmt node.
 */
void Translation::visit(ast::WhileStmt *s) {
    Label L1 = tr->getNewLabel();//当前的
    Label L2 = tr->getNewLabel();//下一条

    Label old_break = current_break_label;
    current_break_label = L2;

    Label old_cont = current_cont_label;
    current_cont_label = L1;
    
    tr->genMarkLabel(L1);
    s->condition->accept(this);
    tr->genJumpOnZero(L2, s->condition->ATTR(val));

    s->loop_body->accept(this);
    tr->genJump(L1);

    tr->genMarkLabel(L2);

    //循环结束完以后old_break恢复到了原来的，这是为了解决循环的嵌套准备的
    current_break_label = old_break;
    current_break_label = old_cont;
}

void Translation::visit(ast::DoWhileStmt *s) {
    Label L1 = tr->getNewLabel();
    Label L2 = tr->getNewLabel();

    Label old_break = current_break_label;
    current_break_label = L2;

    Label old_cont = current_cont_label;
    current_cont_label = L1;
    
    tr->genMarkLabel(L1);
    s->loop_body->accept(this);
    s->condition->accept(this);
    tr->genJumpOnZero(L2, s->condition->ATTR(val));
    tr->genJump(L1);
    tr->genMarkLabel(L2);

    current_break_label = old_break;
    current_cont_label = old_cont;
}

void Translation::visit(ast::ForStmt *s) {

    if(s->condition1!=NULL) s->condition1->accept(this);
    if(s->decl1!=NULL) s->decl1->accept(this);

    Label L1 = tr->getNewLabel();
    Label L2 = tr->getNewLabel();
    Label L3 = tr->getNewLabel();

    Label old_break = current_break_label;
    current_break_label = L2;

    Label old_cont = current_cont_label;
    current_cont_label = L3;
    
    tr->genMarkLabel(L1);
    
    if(s->condition2!=NULL){
        s->condition2->accept(this);
        tr->genJumpOnZero(L2, s->condition2->ATTR(val));
    }
    else{
        Temp temp = tr->getNewTempI4();
        tr->genAssign(temp, tr->genLoadImm4(1));
        tr->genJumpOnZero(L2, temp);
    }
    
    s->loop_body->accept(this);
    tr->genJump(L3);

    tr->genMarkLabel(L3);
    if(s->condition3!=NULL) s->condition3->accept(this);
    tr->genJump(L1);

    tr->genMarkLabel(L2);
    current_break_label = old_break;
    current_cont_label = old_cont;
}

/* Translating an ast::BreakStmt node.
 */
void Translation::visit(ast::BreakStmt *s) { tr->genJump(current_break_label); }


void Translation::visit(ast::ContStmt *s) { tr->genJump(current_cont_label); }
/* Translating an ast::CompStmt node.
 */
void Translation::visit(ast::CompStmt *c) {
    // translates statement by statement
    for (auto it = c->stmts->begin(); it != c->stmts->end(); ++it)
        (*it)->accept(this);
}
/* Translating an ast::ReturnStmt node.
 */
void Translation::visit(ast::ReturnStmt *s) {
    s->e->accept(this);
    tr->genReturn(s->e->ATTR(val));
}

/* Translating an ast::AddExpr node.
 */
void Translation::visit(ast::AddExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genAdd(e->e1->ATTR(val), e->e2->ATTR(val));
    e->ATTR(value)=e->e1->ATTR(value)+e->e2->ATTR(value);
}

/* Translating an ast::SubExor node.
 */
void Translation::visit(ast::SubExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genSub(e->e1->ATTR(val), e->e2->ATTR(val));
    e->ATTR(value)=e->e1->ATTR(value)-e->e2->ATTR(value);
}


/* Translating an ast::MulExpr node.
 */
void Translation::visit(ast::MulExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genMul(e->e1->ATTR(val), e->e2->ATTR(val));
    e->ATTR(value)=e->e1->ATTR(value)*e->e2->ATTR(value);
}

/* Translating an ast::LesExpr node.
 */
void Translation::visit(ast::LesExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genLes(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e1->ATTR(value)<e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::GrtExpr node.
 */
void Translation::visit(ast::GrtExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genGtr(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e1->ATTR(value)>e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::LeqExpr node.
 */
void Translation::visit(ast::LeqExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genLeq(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e1->ATTR(value)<=e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::GeqExpr node.
 */
void Translation::visit(ast::GeqExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genGeq(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e1->ATTR(value)>=e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}


/* Translating an ast::EquExpr node.
 */
void Translation::visit(ast::EquExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genEqu(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e1->ATTR(value)==e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::NeqExpr node.
 */
void Translation::visit(ast::NeqExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genNeq(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e1->ATTR(value)!=e->e2->ATTR(value)) e->ATTR(value)=1;
    else e->ATTR(value)=0;
}

/* Translating an ast::AndExpr node.
 */
void Translation::visit(ast::AndExpr *e) {
    Temp temp = tr->getNewTempI4();
    Label L1=tr->getNewLabel();
    Label L2=tr->getNewLabel();
    e->e1->accept(this);
    tr->genJumpOnZero(L1,e->e1->ATTR(val));
    e->e2->accept(this);
    tr->genAssign(temp, tr->genLAnd(e->e1->ATTR(val), e->e2->ATTR(val)));
    
    //e->ATTR(val)= tr->genLAnd(e->e1->ATTR(val), e->e2->ATTR(val));
    tr->genJump(L2);
    tr->genMarkLabel(L1);
    tr->genAssign(temp, tr->genLoadImm4(0));
    
    //e->ATTR(val)= tr->genLoadImm4(0);
    tr->genMarkLabel(L2);
    e->ATTR(val)= temp;
    e->ATTR(value)=e->e1->ATTR(value)&&e->e2->ATTR(value);
}

/* Translating an ast::OrExpr node.
 */
void Translation::visit(ast::OrExpr *e) {
    Temp temp = tr->getNewTempI4();
    Label L1=tr->getNewLabel();
    Label L2=tr->getNewLabel();
    e->e1->accept(this);
    tr->genJumpOnZero(L1,e->e1->ATTR(val));
    tr->genAssign(temp, tr->genLoadImm4(1));
    
    //e->ATTR(val)= tr->genLoadImm4(1);
    tr->genJump(L2);
    tr->genMarkLabel(L1);
    e->e2->accept(this);
    tr->genAssign(temp, tr->genLOr(e->e1->ATTR(val), e->e2->ATTR(val)));
    
    //e->ATTR(val)= tr->genLOr(e->e1->ATTR(val), e->e2->ATTR(val));
    tr->genMarkLabel(L2);
    e->ATTR(val)= temp;
    e->ATTR(value)=e->e1->ATTR(value)||e->e2->ATTR(value);
}
/* Translating an ast::DivExpr node.
 */
void Translation::visit(ast::DivExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genDiv(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e2->ATTR(value)!=0)
        e->ATTR(value)=e->e1->ATTR(value)/e->e2->ATTR(value);
    else
        e->ATTR(value)=1;
}


/* Translating an ast::ModExpr node.
 */
void Translation::visit(ast::ModExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    e->ATTR(val) = tr->genMod(e->e1->ATTR(val), e->e2->ATTR(val));
    if(e->e2->ATTR(value)!=0)
        e->ATTR(value)=e->e1->ATTR(value)%e->e2->ATTR(value);
    else
        e->ATTR(value)=1;
}


/* Translating an ast::IntConst node.
 */
void Translation::visit(ast::IntConst *e) {
    e->ATTR(val) = tr->genLoadImm4(e->value);
    e->ATTR(value)=e->value;
}

/* Translating an ast::NegExpr node.
 */
void Translation::visit(ast::NegExpr *e) {
    e->e->accept(this);

    e->ATTR(val) = tr->genNeg(e->e->ATTR(val));
    e->ATTR(value)=-(e->e->ATTR(value));
}

void Translation::visit(ast::FNegExpr *e) {
    e->e->accept(this);

    e->ATTR(val) = e->e->ATTR(val);
    e->ATTR(value)=-(e->e->ATTR(value));
}
/* Translating an ast::NotExpr node.
 */
void Translation::visit(ast::NotExpr *e) {
    e->e->accept(this);

    e->ATTR(val) = tr->genLNot(e->e->ATTR(val));

    e->ATTR(value)=~(e->e->ATTR(value));
}
/* Translating an ast::BitNotExpr node.
 */
void Translation::visit(ast::BitNotExpr *e) {
    e->e->accept(this);

    e->ATTR(val) = tr->genBNot(e->e->ATTR(val));

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
            if(ref->ATTR(sym)->isGlobalVar()){
                // if(ref->ATTR(sym)->iscon){
                //     e->ATTR(val) = tr->genLoadImm4(ref->ATTR(sym)->value_v);
                // }
                // else{
                    // Temp temp = tr->genLoadSymbol(ref->ATTR(sym)->getName());
                    // if(ref->ATTR(lv_kind) == ast::Lvalue::ARRAY_ELE)
                    // temp = tr->genAdd(temp, ref->ldim->ATTR(val));
                    // e->ATTR(val) = tr->genLoad(temp, 0);


                    Temp temp = tr->genLoadSymbol(ref->ATTR(sym)->getName());
                    if(ref->ATTR(lv_kind) == ast::Lvalue::ARRAY_ELE){
                        temp = tr->genAdd(temp, ref->ldim->ATTR(val));
                        e->ATTR(val) = tr->genLoad(temp, 0);
                    }
                    else{
                        if(ref->ATTR(type)->isArrayType()){
                            e->ATTR(val) = temp;
                        }
                        else{
                            e->ATTR(val) = tr->genLoad(temp, 0);
                        }
                    }
                       

                    // if(ref->ATTR(type)->isArrayType())
                    // {
                    //     if(ref->ldim==NULL){
                    //         e->ATTR(val) = temp;
                    //     }
                    //     else{
                    //         temp = tr->genAdd(temp, ref->ldim->ATTR(val));
                    //         e->ATTR(val) = tr->genLoad(temp, 0);
                    //     }
                    // }
                    // else{
                    //     // temp = tr->genAdd(temp, ref->ldim->ATTR(val));
                    //     e->ATTR(val) = tr->genLoad(temp, 0);
                    // }
                
                // if(ref->ATTR(sym)->iscon){
                //     e->ATTR(value) = ref->ATTR(sym)->con_val;
                // }
                // else{
                //     scope::Scope *scop_temp=ref->ATTR(sym)->getScope();
                //     int jiji=0;
                //     for(auto item=scop_temp->begin();item!=scop_temp->end();item++){
                //         if((*item)->getName()==ref->var)
                //         {
                //             e->ATTR(value)=(dynamic_cast<mind::symb::Variable *>(*item))->getGlobalInit();
                //             jiji=1;
                //         }
                //     }
                //     if(jiji==0) mind_assert(false);
                // }
            }
            else {
                if(ref->ATTR(sym)->iscon){
                    e->ATTR(val) = tr->genLoadImm4(ref->ATTR(sym)->value_v);
                }
                else{
                    if(ref->ATTR(lv_kind) == ast::Lvalue::ARRAY_ELE){
                        Temp temp = tr->genAdd(ref->ATTR(sym)->getTemp(), ref->ldim->ATTR(val));
                        e->ATTR(val) = tr->genLoad(temp, 0);
                    }
                    else 
                        e->ATTR(val) = ref->ATTR(sym)->getTemp();
                }
                
            }
            
            e->ATTR(value)=ref->ATTR(sym)->value_v;

            break;
        }
        default:
            mind_assert(false);
    }
}


void Translation::visit(ast::CallExpr *e) {
    std::string a="putint";
    std::string b="putch";
    std::string c="getarray";
    std::string d="putarray";
    if(e->name==a||e->name==b||e->name==c||e->name==d){
        for(auto expr : *(e->elist)){
            // std::cout<<((ast::LvalueExpr *)expr)->ATTR(value);
            expr->accept(this); 
            assert(expr->ATTR(val) != NULL);
            tr->genPush1(expr->ATTR(val));
        }
        e->ATTR(val) = tr->genCall(e->ATTR(sym)->getEntryLabel());
        assert(e->ATTR(val) != NULL);
    }
    else{
        for(auto expr : *(e->elist)){
            // std::cout<<((ast::LvalueExpr *)expr)->ATTR(value);
            expr->accept(this); 
            assert(expr->ATTR(val) != NULL);
            tr->genPush(expr->ATTR(val));
        }
        e->ATTR(val) = tr->genCall(e->ATTR(sym)->getEntryLabel());
        assert(e->ATTR(val) != NULL);
    }

    
    
}

/* Translating an ast::VarRef node.
 *
 * NOTE:
 *   there are two kinds of variable reference: member variables or simple
 * variables
 */

void Translation::visit(ast::IndexExpr *e){
    
    if(e->ATTR(dim)!=NULL){
        auto expr = e->expr_list->begin();
        auto dim = e->ATTR(dim)->begin();
        dim++;
        (*expr)->accept(this);
        Temp temp = (*expr)->ATTR(val); ++expr;
        for(int i = 1; i < e->expr_list->length(); ++expr, ++dim, ++i){
            (*expr)->accept(this);
            // std::cout<<(*dim)<<" ";
            // std::cout<<(*expr)->ATTR(value)<<" ";
            Temp t = tr->genLoadImm4(*dim);
            temp = tr->genMul(temp, t);
            temp = tr->genAdd(temp, (*expr)->ATTR(val));
        }
        Temp t = tr->genLoadImm4(4);
        temp = tr->genMul(temp, t);
        e->ATTR(val) = temp;
    }
    else{
        
        auto expr = e->expr_list->begin();
        (*expr)->accept(this);
        Temp temp = (*expr)->ATTR(val); ++expr;
        Temp t = tr->genLoadImm4(4);
        temp = tr->genMul(temp, t);
        e->ATTR(val) = temp;
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

/* Translating an ast::VarDecl node.
 */
void Translation::visit(ast::VarDecl *decl) {
    
    if(decl->ATTR(sym)->isGlobalVar()){
        if(decl->type->ATTR(type)->isArrayType()){
            // if(decl->ATTR(sym)->rdim==NULL){
            //     int a=decl->ATTR(sym)->getType()->getSize();
            //     decl->ATTR(sym)->rdim=new ast::DimList();
            //     // std::cout<<a;
            //     while(a>0){
            //         decl->ATTR(sym)->rdim->append_my(0);
            //         a-=4;
            //     }
            // }
            // else{
            //     int a=decl->ATTR(sym)->getType()->getSize();
            //     int b=decl->ATTR(sym)->rdim->length();
            //     if(a>b){
            //         int c=a-b;
            //         while(c>0){
            //             decl->ATTR(sym)->rdim->append(0);
            //             c-=4;
            //         }
            //     }
            // }
        }
        else{
            
            if(decl->init==NULL){
                decl->ATTR(sym)->setGlobalInit(0);
                decl->ATTR(sym)->value_v=0;
                
            }
            else{
                //decl->ATTR(sym)->setGlobalInit(0);
                decl->init->accept(this);
                
                if(decl->init->getKind() == ast::ASTNode::INT_CONST)
                {
                    decl->ATTR(sym)->setGlobalInit(((ast::IntConst *)(decl->init))->value);
                    decl->ATTR(sym)->value_v=((ast::IntConst *)(decl->init))->value;
                }
                else if(decl->init->getKind() == ast::ASTNode::CALL_EXPR){
                    assert(decl->init->getKind() == ast::ASTNode::INT_CONST);
                }
                else {
                    decl->ATTR(sym)->setGlobalInit(decl->init->ATTR(value));
                    decl->ATTR(sym)->value_v=decl->init->ATTR(value);
                }
                // assert(decl->init->getKind() == ast::ASTNode::INT_CONST);
                // decl->ATTR(sym)->setGlobalInit(((ast::IntConst *)(decl->init))->value);
            }
        }
        
        
    }
    else{
        if(decl->type->ATTR(type)->isArrayType()){
            
            decl->ATTR(sym)->attachTemp(tr->allocNewTempI4(decl->type->ATTR(type)->getSize()));
            if(decl->rrdim!=NULL){
                int a=0;
                for(auto it=decl->rrdim->begin();it!=decl->rrdim->end();it++){
                    // (*it)->accept(this);
                    Temp temp=tr->genLoadImm4((*it));
                    tr->genStore(temp, decl->ATTR(sym)->getTemp(), a);
                    a+=4;
                }
            }
        }


        else{
            decl->ATTR(sym)->attachTemp(tr->getNewTempI4());
            if(decl->init!=NULL){
                decl->init->accept(this);
                decl->ATTR(sym)->value_v=decl->init->ATTR(value);
                tr->genAssign(decl->ATTR(sym)->getTemp(),decl->init->ATTR(val));
            }
        }
        
        
    }
    
    if(decl->lian!=NULL){
            for(ast::DouList::iterator it=decl->lian->begin();
            it!=decl->lian->end();++it){
                (*it)->accept(this);
            }
        }
}



/* Translates an entire AST into a Piece list.
 *
 * PARAMETERS:
 *   tree  - the AST
 * RETURNS:
 *   the result Piece list (represented by the first node)
 */
Piece *MindCompiler::translate(ast::Program *tree) {
    TransHelper *helper = new TransHelper(md);

    tree->accept(new Translation(helper));

    return helper->getPiece();
}

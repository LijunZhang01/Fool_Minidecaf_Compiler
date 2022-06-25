/*****************************************************
 *  Implementation of the first semantic analysis pass.
 *
 *  In the first pass, we will:
 *    1. create appropriate type::Type instances for the types;
 *    2. create and manage scope::Scope instances;
 *    3. create symb::Symbol instances;
 *    4. manage the symbol tables.
 *  After this pass, the ATTR(sym) or ATTR(type) attributs of the visited nodes
 *  should have been set.
 *
 *  Keltin Leung 
 */
#include <iostream>
#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "compiler.hpp"
#include "config.hpp"
#include "scope/scope.hpp"
#include "scope/scope_stack.hpp"
#include "symb/symbol.hpp"
#include "type/type.hpp"
#include "3rdparty/list.hpp"
// #include<iostream>
#include "list"
#include <map>
#include "define.hpp"

#include <string> 
// #include "define.hpp"

using namespace mind;
using namespace mind::scope;
using namespace mind::symb;
using namespace mind::type;
using namespace mind::err;

std::map<std::string,int> conv;
std::list <mind::symb::Function *> mind::aa;
bool con_b=false;
int mind::han=1;
auto b=new ast::DimList();
// mind::util::List<mind::symb::Function *> aa=*(new std::list<mind::symb::Function *>());
/* Pass 1 of the semantic analysis.
 */
// std::list<Tac> mind::canlian;
// std::list<symb::Function> mind::aa;
class SemPass1 : public ast::Visitor {
  public:
    // visiting declarations
    virtual void visit(ast::FuncDefn *);
    virtual void visit(ast::Program *);
    virtual void visit(ast::IfStmt *);
    virtual void visit(ast::WhileStmt *);
    virtual void visit(ast::DoWhileStmt *);
    virtual void visit(ast::ForStmt *);
    virtual void visit(ast::CompStmt *);
    virtual void visit(ast::VarDecl *);
    // visiting types
    virtual void visit(ast::IntType *);
    virtual void visit(ast::IndexExpr *e);
    virtual void visit(ast::AddExpr *e) ;
    virtual void visit(ast::SubExpr *e) ;
    virtual void visit(ast::MulExpr *e) ;
    // virtual void visit(ast::LesExpr *e) ;
    // virtual void visit(ast::GrtExpr *e) ;
    // virtual void visit(ast::LeqExpr *e) ;
    // virtual void visit(ast::GeqExpr *e) ;
    // virtual void visit(ast::EquExpr *e) ;
    // virtual void visit(ast::NeqExpr *e) ;
    // virtual void visit(ast::AndExpr *e) ;
    // virtual void visit(ast::OrExpr *e) ;
    virtual void visit(ast::DivExpr *e) ;
    // virtual void visit(ast::ModExpr *e) ;
    virtual void visit(ast::IntConst *e) ;
    virtual void visit(ast::NegExpr *e) ;
    // virtual void visit(ast::NotExpr *e) ;
    // virtual void visit(ast::BitNotExpr *e) ;
    virtual void visit(ast::LvalueExpr *e) ;
    // virtual void visit(ast::VarRef *ref) ;
};

/* Visiting an ast::Program node.
 *
 * PARAMETERS:
 *   prog  - the ast::Progarm node to visit
 */
void SemPass1::visit(ast::Program *prog) {
    prog->ATTR(gscope) = new GlobalScope();
    scopes->open(prog->ATTR(gscope));

    // visit global variables and each function
    for (auto it = prog->func_and_globals->begin();
         it != prog->func_and_globals->end(); ++it) {
        (*it)->accept(this);
        if ((*it)->getKind() == mind::ast::ASTNode::FUNC_DEFN &&
            std::string("main") ==
                dynamic_cast<mind::ast::FuncDefn *>(*it)->name)
            prog->ATTR(main) =
                dynamic_cast<mind::ast::FuncDefn *>(*it)->ATTR(sym);
    }
    // auto it = prog->func_and_globals->begin();
    // auto t=((ast::FuncDefn *)(*it))->ret_type->ATTR(type);
    // BaseType *t=new type::BaseType("int");
    
    scopes->close(); // close the global scope
}

/* Visiting an ast::FunDefn node.
 *
 * NOTE:
 *   tasks include:
 *   1. build up the Function symbol
 *   2. build up symbols of the parameters
 *   3. build up symbols of the local variables
 *
 *   we will check Declaration Conflict Errors for symbols declared in the SAME
 *   class scope, but we don't check such errors for symbols declared in
 *   different scopes here (we leave this task to checkOverride()).
 * PARAMETERS:
 *   fdef  - the ast::FunDefn node to visit
 */
void SemPass1::visit(ast::FuncDefn *fdef) {
    fdef->ret_type->accept(this);
    Type *t = fdef->ret_type->ATTR(type);

    Function *f = new Function(fdef->name, t , fdef->getLocation());

    if(mind::han==1){
        mind::han=0;
        // mind::FList *aa=new mind::FList();
        Function *f1 = new Function("getint", t, fdef->getLocation());
        scopes->declare(f1);
        // aa->append_my(f1);
        mind::aa.push_front(f1);
        // f1->attachEntryLabel(tr->getNewEntryLabel(f1));
        Function *f2 = new Function("getch", t, fdef->getLocation());
        scopes->declare(f2);
        mind::aa.push_front(f2);
        Function *f3 = new Function("getarray", t, fdef->getLocation());
        scopes->declare(f3);
        // aa->append_my(f3);
        mind::aa.push_front(f3);
        Function *f4 = new Function("putint", t, fdef->getLocation());
        scopes->declare(f4);
        // aa->append_my(f4);
        mind::aa.push_front(f4);
        f4->appendParameter(new symb::Variable("",t,fdef->getLocation()));
        Function *f5 = new Function("putch", t, fdef->getLocation());
        scopes->declare(f5);
        // aa->append_my(f5);
        mind::aa.push_front(f5);
        f5->appendParameter(new symb::Variable("",t,fdef->getLocation()));
        Function *f6 = new Function("stoptime", t, fdef->getLocation());
        scopes->declare(f6);
        // aa->append_my(f6);
        mind::aa.push_front(f6);
        Function *f7 = new Function("putarray", t, fdef->getLocation());
        scopes->declare(f7);
        // aa->append_my(f7);
        mind::aa.push_front(f7);
        Function *f8 = new Function("starttime", t, fdef->getLocation());
        scopes->declare(f8);
        // aa->append_my(f8);
        mind::aa.push_front(f8);
        // std::cout<<mind::aa.size();
    }

    fdef->ATTR(sym) = f;

    // checks the Declaration Conflict Error of Case 1 (but don't check Case
    // 2,3). if DeclConflictError occurs, we don't put the symbol into the
    // symbol table
    Symbol *sym = scopes->lookup(fdef->name, fdef->getLocation(), false);
    if (NULL != sym)
        issue(fdef->getLocation(), new DeclConflictError(fdef->name, sym));
    else
        scopes->declare(f);
    
    // opens function scope
    scopes->open(f->getAssociatedScope());

    // adds the parameters
    for (ast::VarList::iterator it = fdef->formals->begin();
         it != fdef->formals->end(); ++it) {
        (*it)->accept(this);
        f->appendParameter((*it)->ATTR(sym));
    }
    
    // adds the local variables
    for (auto it = fdef->stmts->begin(); it != fdef->stmts->end(); ++it)
        (*it)->accept(this);

    // closes function scope
    scopes->close();
}

/* Visits an ast::IfStmt node.
 *
 * PARAMETERS:
 *   e     - the ast::IfStmt node
 */
void SemPass1::visit(ast::IfStmt *s) {
    s->condition->accept(this);
    s->true_brch->accept(this);
    s->false_brch->accept(this);
}

/* Visits an ast::WhileStmt node.
 *
 * PARAMETERS:
 *   e     - the ast::WhileStmt node
 */
void SemPass1::visit(ast::WhileStmt *s) {
    s->condition->accept(this);
    s->loop_body->accept(this);
}


void SemPass1::visit(ast::DoWhileStmt *s) {
    s->loop_body->accept(this);
    s->condition->accept(this);
    
}

void SemPass1::visit(ast::ForStmt *s) {
    Scope *scope = new LocalScope();
    s->ATTR(scope) = scope;
    scopes->open(scope);
    
    if(s->decl1!=NULL) s->decl1->accept(this);
    if(s->condition1!=NULL) s->condition1->accept(this);
    if(s->condition2!=NULL) s->condition2->accept(this);
    s->loop_body->accept(this);
    if(s->condition3!=NULL) s->condition3->accept(this);
    scopes->close();
}
/* Visiting an ast::CompStmt node.
 */
void SemPass1::visit(ast::CompStmt *c) {
    // opens function scope
    Scope *scope = new LocalScope();
    c->ATTR(scope) = scope;
    scopes->open(scope);

    // adds the local variables
    for (auto it = c->stmts->begin(); it != c->stmts->end(); ++it)
        (*it)->accept(this);

    // closes function scope
    scopes->close();
}
/* Visiting an ast::VarDecl node.
 *
 * NOTE:
 *   we will check Declaration Conflict Errors for symbols declared in the SAME
 *   function scope, but we don't check such errors for symbols declared in
 *   different scopes here (we leave this task to checkOverride()).
 * PARAMETERS:
 *   vdecl - the ast::VarDecl node to visit
 */
void SemPass1::visit(ast::VarDecl *vdecl) {
    

    Type *t = NULL;

    vdecl->type->accept(this);
    if(vdecl->rdim!=NULL){
        vdecl->rrdim=new ast::DimList();
        for(auto it=vdecl->rdim->begin();it!=vdecl->rdim->end();it++){
            (*it)->accept(this);
            // std::cout<<(*it)->ATTR(con_v)<<" ";
            vdecl->rrdim->append((*it)->ATTR(con_v));
        }
    }
    
    if(vdecl->ldim!=NULL){
        con_b=true;
        vdecl->ldim->accept(this);
        int length = 1;
        for(int d : *(vdecl->ldim->ATTR(dim1))){
            length *= d;
        }
        if(length == 0){
            issue(vdecl->getLocation(), new ZeroLengthedArrayError());
            return ;
        }
        // std::cout<<length;
        int d=vdecl->ldim->ATTR(dim1)->length();
        
        vdecl->type->ATTR(type) = new ArrayType(vdecl->type->ATTR(type), length,d);
    }
    con_b=false;
    // if(vdecl->ldim != NULL) {
    //     int length = 1;
    //     for(int d : *(vdecl->ldim)){
    //         length *= d;
    //     }
    //     if(length == 0){
    //         issue(vdecl->getLocation(), new ZeroLengthedArrayError());
    //         return ;
    //     }
    //     int d=vdecl->ldim->length();
    //     vdecl->type->ATTR(type) = new ArrayType(vdecl->type->ATTR(type), length,d);
    // }

    
    if(vdecl->can){
        // ast::DimList *a=new ast::DimList();
        vdecl->type->ATTR(type) = new ArrayType(vdecl->type->ATTR(type), 10,1);
        // Type *mm=new ArrayType(vdecl->type->ATTR(type), 10,1);
        // v=new Variable(vdecl->name, mm, NULL,NULL,con,vdecl->getLocation());
    }
    t = vdecl->type->ATTR(type);
    // TODO: Add a new symbol to a scope
    // 1. Create a new `Variable` symbol
    // 2. Check for conflict in `scopes`, which is a global variable refering to
    // a scope stack
    // 3. Declare the symbol in `scopes`
    // 4. Special processing for global variables
    // 5. Tag the symbol to `vdecl->ATTR(sym)`
    std::int16_t con=vdecl->const1;
    Variable *v;
    if(vdecl->ldim==NULL){
        v=new Variable(vdecl->name, t,NULL,vdecl->rrdim,con,vdecl->getLocation());
    }
    
    else{
        v=new Variable(vdecl->name, t, vdecl->ldim->ATTR(dim1),vdecl->rrdim,con,vdecl->getLocation());
    }
    // Variable *v=new Variable(vdecl->name, t, vdecl->ldim->ATTR(dim1),vdecl->rdim,con,vdecl->getLocation());
    if(con){
        if(vdecl->rdim==NULL){
            if(vdecl->init!=NULL){
                con_b=true;
                vdecl->init->accept(this);
                conv[vdecl->name]=vdecl->init->ATTR(con_v);
            }
            con_b=false;
        }
        else{
            int t=1;
            for(auto it=vdecl->rrdim->begin();it!=vdecl->rrdim->end();it++){
                conv[vdecl->name+std::to_string(t)]=(*it);
                t++;
            }

            while(t<(((ArrayType *)(vdecl->type->ATTR(type)))->getLength())){
                conv[vdecl->name+std::to_string(t)]=0;
                t++;
            }
        }
        
    }
    if(vdecl->can){
        // ast::DimList *a=new ast::DimList();
        
        v=new Variable(vdecl->name, t, NULL,NULL,con,vdecl->getLocation());
    }
    vdecl->ATTR(sym) = v;
    Symbol *sym = scopes->lookup(vdecl->name, vdecl->getLocation(), 0);
    if(sym != NULL)
        issue(vdecl->getLocation(),new DeclConflictError(vdecl->name, sym));
    else
        scopes->declare(vdecl->ATTR(sym));
        
    // //为全局变量赋初值
    // if(v->isGlobalVar()){
    //     v->setGlobalInit(0);
    // }
    
    if(vdecl->lian!=NULL)
    {   for(ast::DouList::iterator it=vdecl->lian->begin();
        it!=vdecl->lian->end();++it){
            (*it)->accept(this);
        }
    }
}

/* Visiting an ast::IntType node.
 *
 * PARAMETERS:
 *   itype - the ast::IntType node to visit
 */
void SemPass1::visit(ast::IntType *itype) { itype->ATTR(type) = BaseType::Int; }

/* Builds the symbol tables for the Mind compiler.
 *
 * PARAMETERS:
 *   tree  - the AST of the program
 */
void MindCompiler::buildSymbols(ast::Program *tree) {
    tree->accept(new SemPass1());
}


void SemPass1::visit(ast::IndexExpr *e) {
    e->ATTR(dim1)=new ast::DimList();
    for(auto c : *(e->expr_list)){
        c->accept(this);
        e->ATTR(dim1)->append(c->ATTR(con_v)); 
    }
        
}


void SemPass1::visit(ast::IntConst *e) {
    e->ATTR(con_v)=e->value;
}

void SemPass1::visit(ast::AddExpr *e) {
    if(con_b){
        e->e1->accept(this);
        e->e2->accept(this);
        e->ATTR(con_v)=e->e1->ATTR(con_v)+e->e2->ATTR(con_v);
    }
    
}

void SemPass1::visit(ast::SubExpr *e) {
    if(con_b){
        e->e1->accept(this);
        e->e2->accept(this);

        e->ATTR(con_v)=e->e1->ATTR(con_v)-e->e2->ATTR(con_v);
    }
    
}


void SemPass1::visit(ast::MulExpr *e) {
    if(con_b){
        e->e1->accept(this);
        e->e2->accept(this);
        e->ATTR(con_v)=e->e1->ATTR(con_v)*e->e2->ATTR(con_v);
    }
    
}


void SemPass1::visit(ast::LvalueExpr *e){
    if(e->lvalue->ATTR(lv_kind)==0){
        if(con_b){
            std::string temp=((ast::VarRef *)e->lvalue)->var;
            e->ATTR(con_v)=conv[temp];
        }
    }
    else{
        if(con_b){
            std::string temp=((ast::VarRef *)e->lvalue)->var;
            e->ATTR(con_v)=conv[temp];
        }
    }
    
    
}

void SemPass1::visit(ast::NegExpr *e) {
    e->e->accept(this);

    e->ATTR(con_v)=-(e->e->ATTR(con_v));
}

void SemPass1::visit(ast::DivExpr *e) {
    e->e1->accept(this);
    e->e2->accept(this);

    
    e->ATTR(con_v)=e->e1->ATTR(con_v)/e->e2->ATTR(con_v);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

// void SemPass1::visit(ast::IndexExpr *e) {
//     e->ATTR(dim1)=new ast::DimList();
//     for(auto c : *(e->expr_list)){
//         if(c->getKind()==mind::ast::ASTNode::NodeType::LVALUE_EXPR){
//             Symbol *v = scopes->lookup(((ast::VarRef *)((ast::LvalueExpr *)c)->lvalue)->var, ((ast::VarRef *)((ast::LvalueExpr *)c)->lvalue)->getLocation());
//             if(v->iscon){
//                 c->ATTR(value)=((symb::Variable *)v)->con_val;
//             }
//             else{
//                 mind_assert(false);
//             }
            
//         }
//         else{
//             c->accept(this);
//         }
//         e->ATTR(dim1)->append(c->ATTR(value));
        
//     }
        
// }

// void SemPass1::visit(ast::AddExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

    
//     e->ATTR(value)=e->e1->ATTR(value)+e->e2->ATTR(value);
// }

// /* Translating an ast::SubExor node.
//  */
// void SemPass1::visit(ast::SubExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     e->ATTR(value)=e->e1->ATTR(value)-e->e2->ATTR(value);
// }


// /* Translating an ast::MulExpr node.
//  */
// void SemPass1::visit(ast::MulExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     e->ATTR(value)=e->e1->ATTR(value)*e->e2->ATTR(value);
// }

// /* Translating an ast::LesExpr node.
//  */
// void SemPass1::visit(ast::LesExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     if(e->e1->ATTR(value)<e->e2->ATTR(value)) e->ATTR(value)=1;
//     else e->ATTR(value)=0;
// }

// /* Translating an ast::GrtExpr node.
//  */
// void SemPass1::visit(ast::GrtExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     if(e->e1->ATTR(value)>e->e2->ATTR(value)) e->ATTR(value)=1;
//     else e->ATTR(value)=0;
// }

// /* Translating an ast::LeqExpr node.
//  */
// void SemPass1::visit(ast::LeqExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     if(e->e1->ATTR(value)<=e->e2->ATTR(value)) e->ATTR(value)=1;
//     else e->ATTR(value)=0;
// }

// /* Translating an ast::GeqExpr node.
//  */
// void SemPass1::visit(ast::GeqExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     if(e->e1->ATTR(value)>=e->e2->ATTR(value)) e->ATTR(value)=1;
//     else e->ATTR(value)=0;
// }


// /* Translating an ast::EquExpr node.
//  */
// void SemPass1::visit(ast::EquExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     if(e->e1->ATTR(value)==e->e2->ATTR(value)) e->ATTR(value)=1;
//     else e->ATTR(value)=0;
// }

// /* Translating an ast::NeqExpr node.
//  */
// void SemPass1::visit(ast::NeqExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

//     if(e->e1->ATTR(value)!=e->e2->ATTR(value)) e->ATTR(value)=1;
//     else e->ATTR(value)=0;
// }

// /* Translating an ast::AndExpr node.
//  */
// void SemPass1::visit(ast::AndExpr *e) {

//     e->e1->accept(this);
    
//     e->e2->accept(this);

//     e->ATTR(value)=e->e1->ATTR(value)&&e->e2->ATTR(value);
// }

// /* Translating an ast::OrExpr node.
//  */
// void SemPass1::visit(ast::OrExpr *e) {
   
//     e->e1->accept(this);
    
//     e->e2->accept(this);
    
//     e->ATTR(value)=e->e1->ATTR(value)||e->e2->ATTR(value);
// }
// /* Translating an ast::DivExpr node.
//  */
// void SemPass1::visit(ast::DivExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);

    
//     e->ATTR(value)=e->e1->ATTR(value)/e->e2->ATTR(value);
// }


// /* Translating an ast::ModExpr node.
//  */
// void SemPass1::visit(ast::ModExpr *e) {
//     e->e1->accept(this);
//     e->e2->accept(this);


//     e->ATTR(value)=e->e1->ATTR(value)%e->e2->ATTR(value);
// }


// /* Translating an ast::IntConst node.
//  */
// void SemPass1::visit(ast::IntConst *e) {
//     e->ATTR(value)=e->value;
// }

// /* Translating an ast::NegExpr node.
//  */
// void SemPass1::visit(ast::NegExpr *e) {
//     e->e->accept(this);

//     e->ATTR(value)=-(e->e->ATTR(value));
// }
// /* Translating an ast::NotExpr node.
//  */
// void SemPass1::visit(ast::NotExpr *e) {
//     e->e->accept(this);


//     e->ATTR(value)=~(e->e->ATTR(value));
// }
// /* Translating an ast::BitNotExpr node.
//  */
// void SemPass1::visit(ast::BitNotExpr *e) {
//     e->e->accept(this);


//     e->ATTR(value)=!(e->e->ATTR(value));

// }

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


/* Translating an ast::LvalueExpr node.
 *
 * NOTE:
 *   different Lvalue kinds need different translation
 */
// void SemPass1::visit(ast::LvalueExpr *e) {
//     // TODO
//     e->lvalue->accept(this);
//     e->ATTR(value)=((ast::VarRef *)e->lvalue)->m;
// //     e->lvalue->accept(this);
// //     switch (e->lvalue->getKind()) {
// //         case ast::ASTNode::VAR_REF:{
// //             ast::VarRef *ref = (ast::VarRef *)e->lvalue;
// //             // if(ref->ATTR(sym)->iscon){
// //             //     e->ATTR(val) = tr->genLoad(temp, 0);
// //             // }
// //             if(ref->ATTR(sym)->iscon){
// //                     e->ATTR(value) = ref->ATTR(sym)->con_val;
// //             }
// //             else{
// //                 mind_assert(false);
// //             }
            
// //             break;
// //         }
// //         default:
// //             mind_assert(false);
// //     }
// }



// void SemPass1::visit(ast::VarRef *ref) {
    
//     // switch (ref->ATTR(lv_kind)) {
//     // case ast::Lvalue::SIMPLE_VAR:
//     //     // nothing to do
//     //     break;
//     // case ast::Lvalue::ARRAY_ELE:
//     //     ref->ldim->accept(this);
//     //     break;
//     // default:
//     //     mind_assert(false); // impossible
//     // }
//     // // actually it is so simple :-)

//     Symbol *v = scopes->lookup(ref->var, ref->getLocation());
//     if (NULL == v) {
//         issue(ref->getLocation(), new SymbolNotFoundError(ref->var));
//          mind_assert(false);

//     } else if (!v->isVariable()) {
//         issue(ref->getLocation(), new NotVariableError(v));
//         mind_assert(false);

//     } else {
//         auto a = (Variable *)v;
        
//         if(a->iscon){
//             ref->m=a->con_val;
//         }
//         else{
//             mind_assert(false);
//         }
        
        
// }
// }


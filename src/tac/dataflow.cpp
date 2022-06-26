/*****************************************************
 *  Variable Liveness Analysis.
 *
 *  This file contains the implementation of the following 3 functions:
 *  . BasicBlock::computeDefAndLiveUse1
 *  2. FlowGraph::analyzeLiveness
 *  3. BasicBlock::analysisLiveness
 * 
 *  Of course, if you add some new Tacs, 
 *  you are supposed to update the codes at line 38 and line 176.
 *
 */

#include "config.hpp"
#include "tac/flow_graph.hpp"
#include "tac/tac.hpp"
#include "3rdparty/set.hpp"
#include <set>
#include <algorithm>

using namespace mind;
using namespace mind::tac;
using namespace mind::util;


static const std::string EnumStrings[] = {"ASSIGN",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "MOD",
        "EQU",
        "NEQ",
        "LES",
        "LEQ",
        "GTR",
        "GEQ",
        "NEG",
        "LAND",
        "LOR",
        "LNOT",
        "BNOT",
        "MARK",
        "JUMP",
        "JZERO",
        "PUSH",
        "PUSH1",
        "POP",
        "RETURN",
        "LOAD_IMM4",
        "MEMO",
        "CALL",
        "LOADSYMBOL",
        "LOAD",
        "STORE",
        "ALLOC"};


void BasicBlock::updateLU(Temp v) {
    if (NULL != v && !Def->contains(v))
        LiveUse->add(v);
}

void BasicBlock::updateDEF(Temp v) {
    if (NULL != v)
        Def->add(v);
}

void BasicBlock::updateKill(Temp v) {
    if (NULL != v)
        Kill->add(v);
    for(auto rit = Gen->begin(); rit !=Gen->end(); ++rit){
        auto b=(*rit);
        if(Kill->contains(b->t1)||Kill->contains(b->t2)){
            Gen->remove(b);
        }
    }
}


void updateRemove(Set<Rhs> *in,Temp kill){
    for(auto rit = in->begin(); rit !=in->end(); ++rit){
        auto b=(*rit);
        if(kill==(b->t1)){
            in->remove(b);
        }
    }
}


void updateAdd(Set<Rhs> *in,std::string op,Temp t1,Temp t2) {
   
        auto a=new RhsObject();
        a->op=op,a->t1=t1,a->t2=t2;
        in->add(a);
    
    
}

void BasicBlock::updateGen(std::string op,Temp t1,Temp t2) {
    if(!Kill->contains(t1)&&!Kill->contains(t2)){
        auto a=new RhsObject();
        a->op=op,a->t1=t1,a->t2=t2;
        Gen->add(a);
    }
    
}
void differenceFrom1(Set<Rhs> *in,Set<Temp> *kill){
    for(auto rit = in->begin(); rit !=in->end(); ++rit){
        auto b=(*rit);
        if(kill->contains(b->t1)||kill->contains(b->t2)){
            in->remove(b);
        }
    }
}

/* Computes the DEF set and the LiveUse set of this basic block.
 *
 * NOTE: if it is an END-BY-JZERO or END-BY-RETURN block, don't miss
 *       the last use site (i.e. this->var).
 *
 * NOTE: sometimes, op0.var of POP may be NULL...
 *
 * HINT: this subroutine is quite simple, so please don't go into extreme.
 */
void BasicBlock::computeDefAndLiveUse(void) {

    for (Tac *t = tac_chain; t != NULL; t = t->next) {
        switch (t->op_code) {
        case Tac::ASSIGN:
        case Tac::NEG:
        case Tac::LNOT:
        case Tac::BNOT:
        case Tac::LOAD:
            updateLU(t->op1.var);
            updateDEF(t->op0.var);
            break;

        case Tac::ADD:
        case Tac::SUB:
        case Tac::MUL:
        case Tac::DIV:
        case Tac::MOD:
        case Tac::EQU:
        case Tac::NEQ:
        case Tac::LES:
        case Tac::LEQ:
        case Tac::GTR:
        case Tac::GEQ:
        case Tac::LAND:
        case Tac::LOR:
            updateLU(t->op1.var);
            updateLU(t->op2.var);
            updateDEF(t->op0.var);
            break;

        case Tac::POP:
        case Tac::ALLOC:
        case Tac::LOAD_IMM4:
        case Tac::LOADSYMBOL:
            updateDEF(t->op0.var);
            break;

        case Tac::PUSH:
        case Tac::PUSH1:
        case Tac::CALL:
            updateLU(t->op0.var);
            break;

        case Tac::STORE:
            updateLU(t->op0.var);
            updateLU(t->op1.var);
            break;

        default:
            mind_assert(false); // MARK, MEMO, JUMP, JZERO and RETURN will not
                                // appear inside
            break;
        }
    }

    switch (end_kind) {
    case BY_JZERO:
    case BY_RETURN:
        updateLU(var);
        break;

    case BY_JUMP:
        break;

    default:
        mind_assert(false); // unreachable
        break;
    }
}

/* Computes the LiveIn set and LiveOut set of every basic block.
 *
 * HINT: please make sure that you understand this algorithm (and how it is
 * performed), or you might regret in your final exam...
 */
void FlowGraph::analyzeLiveness(void) {
    bool changed = false;
    BasicBlock *b1 = NULL, *b2 = NULL, *b = NULL;
    Set<Temp> *newin = NULL;

    // Step 1. computes Def and LiveUse
    for (int i = 0; i < _n; ++i) {
        getBlock(i)->computeDefAndLiveUse();
    }

    // Step 2. iterates

    changed = true;
    while (changed) {
        changed = false;

        for (reverse_iterator rit = rbegin(); rit != rend(); ++rit) {
            b = *rit;

            // updates LiveOut
            switch (b->end_kind) {
            case BasicBlock::BY_JUMP:
                b1 = getBlock(b->next[0]);
                b->LiveOut = b1->LiveIn;
                break;

            case BasicBlock::BY_JZERO:
                b1 = getBlock(b->next[0]);
                b2 = getBlock(b->next[1]);
                b->LiveOut = b1->LiveIn->unionWith(b2->LiveIn);
                break;

            case BasicBlock::BY_RETURN:
                break;

            default:
                mind_assert(false); // unreachable
            }

            // updates LiveIn
            newin = b->LiveOut->differenceFrom(b->Def)->unionWith(b->LiveUse);
            if (!newin->equal(b->LiveIn)) {
                changed = true;
                b->LiveIn = newin;
            }
        }
    }
}

/* Computes the LiveOut set of every contained TAC.
 *
 * It is the same algorithm as that for CFG liveness analysis, except that
 * this "graph" is only a linked list.
 *
 * NOTE: if it is an END-BY-JZERO or END-BY-RETURN block, don't miss
 *       the last use site (i.e. this->var).
 *
 * HINT: this subroutine is even simpler (and you won't need to compute the DEF
 * set and the LiveUse set of every TAC first. why? :-).
 *
 * REQUIREMENT: please explain how you do it in your report.
 */
void BasicBlock::analyzeLiveness(void) {
    // it should not begin until FlowGraph::anayzeLiveness() is done

    Tac *t, *t_next;

    // Step 1. if tac_chain is empty, we do nothing
    if (NULL == tac_chain)
        return;

    // Step 1. locates the last Tac
    for (t = tac_chain; t->next != NULL; t = t->next)
        ;

    // Step 2. begins with LiveOut of the block
    t->LiveOut = LiveOut->clone();
    if (end_kind == BY_JZERO || end_kind == BY_RETURN)
        t->LiveOut->add(var);

    for (t = t->prev; t != NULL; t = t->prev) {
        t->LiveOut = t->next->LiveOut->clone();
        t_next = t->next;

        switch (t->next->op_code) {
        case Tac::ASSIGN:
        case Tac::NEG:
        case Tac::LNOT:
        case Tac::BNOT:
        case Tac::LOAD:
            if (NULL != t_next->op0.var)
                t->LiveOut->remove(t_next->op0.var);
            t->LiveOut->add(t_next->op1.var);
            break;

        case Tac::ADD:
        case Tac::SUB:
        case Tac::MUL:
        case Tac::DIV:
        case Tac::MOD:
        case Tac::EQU:
        case Tac::NEQ:
        case Tac::LES:
        case Tac::LEQ:
        case Tac::GTR:
        case Tac::GEQ:
        case Tac::LAND:
        case Tac::LOR:
            if (NULL != t_next->op0.var)
                t->LiveOut->remove(t_next->op0.var);
            t->LiveOut->add(t_next->op1.var);
            t->LiveOut->add(t_next->op2.var);
            break;

        case Tac::POP:
        case Tac::ALLOC:
        case Tac::LOAD_IMM4:
        case Tac::LOADSYMBOL:
            if (NULL != t_next->op0.var)
                t->LiveOut->remove(t_next->op0.var);
            break;

        case Tac::PUSH:
        case Tac::PUSH1:
        case Tac::CALL:
            t->LiveOut->add(t_next->op0.var);
            break;

        case Tac::STORE:
            t->LiveOut->add(t_next->op0.var);
            t->LiveOut->add(t_next->op1.var);
            break;

        default:
            mind_assert(false); // MARK, MEMO, JUMP, JZERO and RETURN will not
                                // appear inside
            break;
        }
        // if(t->op_code!=Tac::PUSH1&&
        //     t->op_code!=Tac::PUSH&&
        //     t->op_code!=Tac::STORE&&
        //     t->op_code!=Tac::CALL&&
        //     t->LiveOut->empty()) t->mark=1;
        if(mind::ctrl_sidaima&&t->op_code!=Tac::PUSH1&&
            t->op_code!=Tac::PUSH&&
            t->op_code!=Tac::STORE&&
            t->op_code!=Tac::CALL&&
            !t->LiveOut->contains(t->op0.var)) t->mark=1;
    }
}



//计算每一个块的gen和kill
void BasicBlock::computeGenAndKill(void) {

    for (Tac *t = tac_chain; t != NULL; t = t->next) {
        switch (t->op_code) {
        case Tac::ASSIGN:
        case Tac::NEG:
        case Tac::LNOT:
        case Tac::BNOT:
        case Tac::LOAD:
            updateKill(t->op0.var);
            updateGen(EnumStrings[t->op_code],t->op1.var,t->op2.var);
            break;

        case Tac::ADD:
        case Tac::SUB:
        case Tac::MUL:
        case Tac::DIV:
        case Tac::MOD:
        case Tac::EQU:
        case Tac::NEQ:
        case Tac::LES:
        case Tac::LEQ:
        case Tac::GTR:
        case Tac::GEQ:
        case Tac::LAND:
        case Tac::LOR:
            updateKill(t->op0.var);
            updateGen(EnumStrings[t->op_code],t->op1.var,t->op2.var);
            break;

        case Tac::POP:
            break;
        case Tac::ALLOC:
        case Tac::LOAD_IMM4:
        case Tac::LOADSYMBOL:
            updateKill(t->op0.var);
            break;

        case Tac::PUSH:
        case Tac::PUSH1:
            break;
        case Tac::CALL:
            // updateLU(t->op0.var);
            break;

        case Tac::STORE:
            // updateLU(t->op0.var);
            // updateLU(t->op1.var);
            break;

        default:
            mind_assert(false); // MARK, MEMO, JUMP, JZERO and RETURN will not
                                // appear inside
            break;
        }
    }

    switch (end_kind) {
    case BY_JZERO:
    case BY_RETURN:
        // updateLU(var);
        break;

    case BY_JUMP:
        break;

    default:
        mind_assert(false); // unreachable
        break;
    }
}

//计算每一个基本块的in和out
void FlowGraph::analyzeCOMMEXPRLiveness(void) {
    bool changed = false;
    BasicBlock *b1 = NULL, *b2 = NULL, *b = NULL;
    Set<Temp> *newin = NULL;

    // Step 1. computes Def and LiveUse
    for (int i = 0; i < _n; ++i) {
        getBlock(i)->computeGenAndKill();
    }

    // Step 2. iterates

    changed = true;
    while (changed) {
        changed = false;

        for (iterator rit = begin(); rit != end(); ++rit) {
            b = *rit;

            for(int i=0;i<b->num_prv;i++){
                b->CEIn=getBlock(b->per[i])->CEOut->intersectionWith(b->CEIn);
            }


            differenceFrom1(b->CEIn,b->Kill);
            // updates LiveIn
            auto newin = b->CEIn->unionWith(b->Gen);
            if (!newin->equal(b->CEOut)) {
                changed = true;
                b->CEOut = newin;
            }
        }
    }
}

//更新每一个Tac的in和out
void BasicBlock::analyzeCOMMEXPRLiveness(void) {
    // it should not begin until FlowGraph::anayzeLiveness() is done

    Tac *t, *t_next;

    // Step 1. if tac_chain is empty, we do nothing
    if (NULL == tac_chain)
        return;

    // Step 1. locates the last Tac
    // for (t = tac_chain; t->next != NULL; t = t->next)
    //     ;
    t = tac_chain;
    // Step 2. begins with LiveOut of the block
    t->CEIn = CEIn->clone();
    // if (end_kind == BY_JZERO || end_kind == BY_RETURN)
    //     t->LiveOut->add(var);

    for (t = t->next; t != NULL; t = t->next) {
        t->CEIn = t->prev->CEIn->clone();
        t_next = t->prev;

        switch (t->next->op_code) {
        case Tac::ASSIGN:
        case Tac::NEG:
        case Tac::LNOT:
        case Tac::BNOT:
        case Tac::LOAD:
            if (NULL != t_next->op0.var)
                updateRemove(t->CEIn,t_next->op0.var);
            updateAdd(t->CEIn,EnumStrings[t->next->op_code],t_next->op1.var,t_next->op2.var);
            break;

        case Tac::ADD:
        case Tac::SUB:
        case Tac::MUL:
        case Tac::DIV:
        case Tac::MOD:
        case Tac::EQU:
        case Tac::NEQ:
        case Tac::LES:
        case Tac::LEQ:
        case Tac::GTR:
        case Tac::GEQ:
        case Tac::LAND:
        case Tac::LOR:
            if (NULL != t_next->op0.var)
                updateRemove(t->CEIn,t_next->op0.var);
            updateAdd(t->CEIn,EnumStrings[t->next->op_code],t_next->op1.var,t_next->op2.var);
            break;

        case Tac::POP:
            break;
        case Tac::ALLOC:
        case Tac::LOAD_IMM4:
        case Tac::LOADSYMBOL:
            if (NULL != t_next->op0.var)
                updateRemove(t->CEIn,t_next->op0.var);
            // updateAdd(t->CEIn,t->next->op_code,t_next->op1.var,t_next->op2.var);
            break;

        case Tac::PUSH:
        case Tac::PUSH1:
        case Tac::CALL:
            if (NULL != t_next->op0.var)
                updateRemove(t->CEIn,t_next->op0.var);
            // updateAdd(t->CEIn,t->next->op_code,t_next->op1.var,t_next->op2.var);
            break;

        case Tac::STORE:
            if (NULL != t_next->op0.var)
                updateRemove(t->CEIn,t_next->op0.var);
            break;

        default:
            mind_assert(false); // MARK, MEMO, JUMP, JZERO and RETURN will not
                                // appear inside
            break;
        }
        //if(CEIn->contains())
    }
}

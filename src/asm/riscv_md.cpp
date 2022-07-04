/*****************************************************
 *  Implementation of RiscvDesc.
 *
 */

#include "asm/riscv_md.hpp"
#include "3rdparty/set.hpp"
#include "asm/offset_counter.hpp"
#include "asm/riscv_frame_manager.hpp"
#include "config.hpp"
#include "options.hpp"
#include "scope/scope.hpp"
#include "symb/symbol.hpp"
#include "tac/flow_graph.hpp"
#include "tac/tac.hpp"
#include "list"

#include <cstring>
#include <iomanip>
#include <sstream>
#include<set>
#include<map>
using namespace mind::assembly;
using namespace mind::tac;
using namespace mind::util;
using namespace mind;

// declaration of empty string
#define EMPTY_STR std::string()
#define WORD_SIZE 4

std::set<Temp> nodes;
std::map<Temp, std::set<Temp>> neighbours;
std::map<Temp, int> nodeDeg;
int regs[18]={RiscvReg::T0,
              RiscvReg::T1,
              RiscvReg::T2,
              RiscvReg::T3,
              RiscvReg::T4,
              RiscvReg::T5,
              RiscvReg::T6,
              RiscvReg::S1,
              RiscvReg::S2,
              RiscvReg::S3,
              RiscvReg::S4,
              RiscvReg::S5,
              RiscvReg::S6,
              RiscvReg::S7,
              RiscvReg::S8,
              RiscvReg::S9,
              RiscvReg::S10,RiscvReg::S11};

//额滴优化
bool mind::ctrl_sidaima=true;
bool mind::ctrl_kongzhiliu=true;
bool mind::ctrl_changliang=true;
bool mind::ctrl_jicunqi=true;
bool mind::ctrl_qiangduxueruo=true;
void alloc(tac::BasicBlock *b);
std::list<Tac> mind::canlian;
/* Constructor of RiscvReg.
 *
 * PARAMETERS:
 *   reg_name   - name of this register
 *   is_general - whether this register is a general-purpose one
 */
RiscvReg::RiscvReg(const char *reg_name, bool is_general) {
    name = reg_name;
    dirty = false;
    var = NULL;
    general = is_general;
}

/* Constructor of RiscvDesc.
 *
 */
RiscvDesc::RiscvDesc(void) {
    // {GLOBAL, LOCAL, PARAMETER}
    // Actually, we only use the parameter offset counter,
    // other two options are remained for extension
    int start[3] = {0, 0, 0}; 
    int dir[3] = {+1, -1, +1};
    _counter = new OffsetCounter(start, dir);

    // initializes the register vector
    // (we regard all general-purpose registers as caller-saved, which is
    // different from the Riscv specification)
    _reg[RiscvReg::ZERO] = new RiscvReg("zero", false); // zero
    _reg[RiscvReg::RA] = new RiscvReg("ra", false);     // return address
    _reg[RiscvReg::SP] = new RiscvReg("sp", false);     // stack pointer
    _reg[RiscvReg::GP] = new RiscvReg("gp", false);     // global pointer
    _reg[RiscvReg::TP] = new RiscvReg("tp", false);     // thread pointer
    _reg[RiscvReg::T0] = new RiscvReg("t0", true);
    _reg[RiscvReg::T1] = new RiscvReg("t1", true);
    _reg[RiscvReg::T2] = new RiscvReg("t2", true);
    _reg[RiscvReg::T3] = new RiscvReg("t3", true);
    _reg[RiscvReg::T4] = new RiscvReg("t4", true);
    _reg[RiscvReg::T5] = new RiscvReg("t5", true);
    _reg[RiscvReg::T6] = new RiscvReg("t6", true);
    _reg[RiscvReg::FP] = new RiscvReg("fp", false); // frame pointer
    _reg[RiscvReg::S1] = new RiscvReg("s1", true);
    _reg[RiscvReg::S2] = new RiscvReg("s2", true);
    _reg[RiscvReg::S3] = new RiscvReg("s3", true);
    _reg[RiscvReg::S4] = new RiscvReg("s4", true);
    _reg[RiscvReg::S5] = new RiscvReg("s5", true);
    _reg[RiscvReg::S6] = new RiscvReg("s6", true);
    _reg[RiscvReg::S7] = new RiscvReg("s7", true);
    _reg[RiscvReg::S8] = new RiscvReg("s8", true);
    _reg[RiscvReg::S9] = new RiscvReg("s9", true);
    _reg[RiscvReg::S10] = new RiscvReg("s10", true);
    _reg[RiscvReg::S11] = new RiscvReg("s11", true);
    _reg[RiscvReg::A0] = new RiscvReg("a0", false); // argument, return value
    _reg[RiscvReg::A1] = new RiscvReg("a1", false); // argument, return value
    _reg[RiscvReg::A2] = new RiscvReg("a2", false); // argument
    _reg[RiscvReg::A3] = new RiscvReg("a3", false); // argument
    _reg[RiscvReg::A4] = new RiscvReg("a4", false); // argument
    _reg[RiscvReg::A5] = new RiscvReg("a5", false); // argument
    _reg[RiscvReg::A6] = new RiscvReg("a6", false); // argument
    _reg[RiscvReg::A7] = new RiscvReg("a7", false); // argument

    _lastUsedReg = 0;
    _label_counter = 0;
}

/* Gets the offset counter for this machine.
 *
 * RETURNS:
 *   the offset counter for Riscv
 */



OffsetCounter *RiscvDesc::getOffsetCounter(void) { return _counter; }

/* Translates the given Piece list into assembly code and output.
 *
 * PARAMETERS:
 *   ps    - the Piece list
 *   os    - the output stream
 */
void RiscvDesc::emitPieces(scope::GlobalScope *gscope, Piece *ps,
                           std::ostream &os) {

    _result = &os;
    // output to .data and .bss segment
    std::ostringstream _data, _bss;

    if (Option::getLevel() == Option::ASMGEN) {
        // program preamble
        emit(EMPTY_STR, ".text", NULL);
        


        emit(EMPTY_STR, ".data", NULL);
        for(auto item=gscope->begin();item!=gscope->end();item++)
        {
            if((*item)->isVariable()){
                if((*item)->getType()->isArrayType()){
                    os << "          " << std::left << std::setw(30) << ".global "<<(*item)->getName();
                    os << std::endl;
                    os<<(*item)->getName()<<":";
                    auto itt=dynamic_cast<mind::symb::Variable *>(*item);
                    if(itt->rdim==NULL){
                        os << "          " << std::left << std::setw(30) << ".zero "<<itt->getType()->getSize();
                        os << std::endl;
                    }
                    else{
                        int a=0;
                        for(auto it=itt->rdim->begin();it!=itt->rdim->end();it++){
                            os << "          " << std::left << std::setw(30) << ".word "<<(*it);
                            os << std::endl;
                            a+=4;
                        }
                        int cha=itt->getType()->getSize()-a;
                        if(a>0){
                            os << "          " << std::left << std::setw(30) << ".zero "<<cha;
                            os << std::endl;
                        }
                    }
            
                    // auto itt=dynamic_cast<mind::symb::Variable *>(*item);
                    
                    // for(auto it=itt->rdim->begin();it!=itt->rdim->end();it++){
                    //     os << "          " << std::left << std::setw(30) << ".word "<<(*it);
                    //     os << std::endl;
                    // }
                    
                    
                }
                else{
                    os << "          " << std::left << std::setw(30) << ".global "<<(*item)->getName();
                    os << std::endl;
                    os<<(*item)->getName()<<":";
                    os << std::endl;
                    os << "          " << std::left << std::setw(30) << ".word "<<(dynamic_cast<mind::symb::Variable *>(*item))->getGlobalInit();
                    os << std::endl;
                }
                
            }
        }
            
            
        // os << "          " << std::left << std::setw(30) << ".global ";

        // for(auto item=gscope->begin();item!=gscope->end();item++){
            
        //     if((*item)->isVariable()){
        //         os << (*item)->getName() << std::left << std::setw(40 - (*item)->getName().length()) << ":";
        //         os << std::endl;
        //         os << "    ";
        //         os << ".word" << std::left << std::setw(40 - std::string(".word").length()) << " ";
        //         os<<(dynamic_cast<mind::symb::Variable *>(*item))->getGlobalInit();
        //     }
        // }
        
        

        
        emit(EMPTY_STR, ".globl main", NULL);
        emit(EMPTY_STR, ".align 2", NULL);
    }
    // translates node by node

    while (NULL != ps) {
        switch (ps->kind) {
        case Piece::FUNCTY:
        
            emitFuncty(ps->as.functy);
            break;

        default:
            mind_assert(false); // unreachable
            break;
        }

        ps = ps->next;
    }
}

/* Allocates a new label (for a basic block).
 *
 * RETURNS:
 *   a new label guaranteed to be non-conflict with the existing ones
 */
const char *RiscvDesc::getNewLabel(void) {
    mind_assert(_label_counter < 10000);

    char *buf = new char[10];
    std::sprintf(buf, "__LL%d", _label_counter++);

    return buf;
}

/* Translates a single basic block into Riscv instructions.
 *
 * PARAMETERS:
 *   b     - the basic block to translate
 *   g     - the control-flow graph
 * RETURNS:
 *   the Riscv instruction sequence of this basic block
 */
RiscvInstr *RiscvDesc::prepareSingleChain(BasicBlock *b, FlowGraph *g) {
    RiscvInstr leading;
    int r0;

    _tail = &leading;
    for (Tac *t = b->tac_chain; t != NULL; t = t->next)
        emitTac(t);

    switch (b->end_kind) {
    case BasicBlock::BY_JUMP:
        spillDirtyRegs(b->LiveOut);
        addInstr(RiscvInstr::J, NULL, NULL, NULL, 0,
                 std::string(g->getBlock(b->next[0])->entry_label), NULL);
        // "B" for "branch"
        break;

    case BasicBlock::BY_JZERO:
        r0 = getRegForRead(b->var, 0, b->LiveOut);
        spillDirtyRegs(b->LiveOut);
        // uses "branch if equal to zero" instruction
        addInstr(RiscvInstr::BEQZ, _reg[r0], NULL, NULL, 0,
                 std::string(g->getBlock(b->next[0])->entry_label), NULL);
        addInstr(RiscvInstr::J, NULL, NULL, NULL, 0,
                 std::string(g->getBlock(b->next[1])->entry_label), NULL);
        break;

    case BasicBlock::BY_RETURN:
        r0 = getRegForRead(b->var, 0, b->LiveOut);
        spillDirtyRegs(b->LiveOut); // just to deattach all temporary variables
        addInstr(RiscvInstr::MOVE, _reg[RiscvReg::A0], _reg[r0], NULL, 0,
                 EMPTY_STR, NULL);
        addInstr(RiscvInstr::MOVE, _reg[RiscvReg::SP], _reg[RiscvReg::FP], NULL,
                 0, EMPTY_STR, NULL);
        addInstr(RiscvInstr::LW, _reg[RiscvReg::RA], _reg[RiscvReg::FP], NULL,
                 -4, EMPTY_STR, NULL);
        addInstr(RiscvInstr::LW, _reg[RiscvReg::FP], _reg[RiscvReg::FP], NULL,
                 -8, EMPTY_STR, NULL);
        addInstr(RiscvInstr::RET, NULL, NULL, NULL, 0, EMPTY_STR, NULL);
        break;

    default:
        mind_assert(false); // unreachable
    }
    _tail = NULL;
    return leading.next;
}

/* Translates a single TAC into Riscv instructions (and records the result.
 *
 * PARAMETERS:
 *   t     - the TAC to translate
 * SIDE-EFFECT:
 *   modifies the "_tail" field
 */
void RiscvDesc::emitAllocTac(Tac *t) {
    if (!t->LiveOut->contains(t->op0.var))
        return;
    int r0 = getRegForWrite(t->op0.var, 0, 0, t->LiveOut);
    addInstr(RiscvInstr::ADDI, _reg[RiscvReg::SP], _reg[RiscvReg::SP], NULL, -t->op1.size, EMPTY_STR, NULL);
    addInstr(RiscvInstr::MOVE, _reg[r0], _reg[RiscvReg::SP], NULL, 0, EMPTY_STR, NULL);
}
void RiscvDesc::emitTac(Tac *t) {
    if(t->mark!=1){
        std::ostringstream oss;
    t->dump(oss);
    addInstr(RiscvInstr::COMMENT, NULL, NULL, NULL, 0, EMPTY_STR, oss.str().c_str() + 4);

    switch (t->op_code) {
    case Tac::ALLOC:
        emitAllocTac(t);
        break;
    case Tac::LOADSYMBOL:
        emitLaTac(RiscvInstr::LA,t);
        break;

    case Tac::LOAD:
        
        emitLwTac(RiscvInstr::LW,t);
        break;
    case Tac::STORE:
        
        emitSwTac(RiscvInstr::SW,t);
        break;
    case Tac::CALL:
        emitCallTac(RiscvInstr::CALL,t);
        break;
    case Tac::PUSH:
        //顺序原因，什么都不做。emitCallTac会进行调用emitPush调用
        emitPushTac(t);
        break;
    case Tac::PUSH1:
        //顺序原因，什么都不做。emitCallTac会进行调用emitPush调用
        emitPushTac1(t);
        break;
    case Tac::LOAD_IMM4:
        emitLoadImm4Tac(t);
        break;

    case Tac::NEG:
        emitUnaryTac(RiscvInstr::NEG, t);
        break;
    
    case Tac::ADD:
        emitBinaryTac(RiscvInstr::ADD, t);
        break;

    case Tac::SUB:
        emitBinaryTac(RiscvInstr::SUB, t);
        break;

    case Tac::MUL:
        
        emitBinaryTac(RiscvInstr::MUL, t);
        break;

    case Tac::DIV:
        emitBinaryTac(RiscvInstr::DIV, t);
        break;

    case Tac::MOD:
        emitBinaryTac(RiscvInstr::MOD, t);
        break;

    case Tac::LNOT:
        emitUnaryTac(RiscvInstr::NOT, t);
        break;

    case Tac::BNOT:
        emitUnaryTac(RiscvInstr::SEQZ, t);
        break;
    
    case Tac::LEQ:
        emitBinaryTac(RiscvInstr::LEQ, t);
        break;

    case Tac::GEQ:
        emitBinaryTac(RiscvInstr::GEQ, t);
        break;

    case Tac::LES:
        emitBinaryTac(RiscvInstr::LES, t);
        break;

    case Tac::GTR:
        emitBinaryTac(RiscvInstr::GTR, t);
        break;

    case Tac::EQU:
        emitBinaryTac(RiscvInstr::EQU, t);
        break;

    case Tac::NEQ:
        emitBinaryTac(RiscvInstr::NEQ, t);
        break;

    case Tac::LAND:
        emitBinaryTac(RiscvInstr::LAND, t);
        break;

    case Tac::LOR:
        emitBinaryTac(RiscvInstr::LOR, t);
        break;
    
    case Tac::ASSIGN:
        emitAssignTac(RiscvInstr::ASSIGN, t);
        break;

    default:
        mind_assert(false); // should not appear inside a basic block
    }
    }
    
}

/* Translates a LoadImm4 TAC into Riscv instructions.
 *
 * PARAMETERS:
 *   t     - the LoadImm4 TAC
 */

void RiscvDesc::emitLaTac(RiscvInstr::OpCode op,Tac *t) {
    // eliminates useless assignments
    if (!t->LiveOut->contains(t->op0.var))
        return;

    // uses "load immediate number" instruction
    int r0 = getRegForWrite(t->op0.var, 0, 0, t->LiveOut);
    addInstr(RiscvInstr::LA, _reg[r0], NULL, NULL,0,t->op1.name,
             NULL);
}

void RiscvDesc::emitLwTac(RiscvInstr::OpCode op,Tac *t) {
     if (!t->LiveOut->contains(t->op0.var))
        return;

    // uses "load immediate number" instruction
    int r1 = getRegForRead(t->op1.var, 0, t->LiveOut);
    int r0 = getRegForWrite(t->op0.var, r1, 0, t->LiveOut);
    addInstr(RiscvInstr::LW, _reg[r0], _reg[r1], NULL, t->op1.offset, EMPTY_STR,
             NULL);
}

void RiscvDesc::emitSwTac(RiscvInstr::OpCode op,Tac *t) {
   
    int r0 = getRegForRead(t->op0.var, 0, t->LiveOut);
    int r1 = getRegForRead(t->op1.var, r0, t->LiveOut);
    addInstr(RiscvInstr::SW, _reg[r0], _reg[r1], NULL, t->op1.offset, EMPTY_STR,
             NULL);
}


void RiscvDesc::emitLoadImm4Tac(Tac *t) {
    // eliminates useless assignments
    if (!t->LiveOut->contains(t->op0.var))
        return;

    // uses "load immediate number" instruction
    int r0 = getRegForWrite(t->op0.var, 0, 0, t->LiveOut);
    addInstr(RiscvInstr::LI, _reg[r0], NULL, NULL, t->op1.ival, EMPTY_STR,
             NULL);
}

void RiscvDesc::emitAssignTac(RiscvInstr::OpCode op, Tac *t) {
    // eliminates useless assignments
    if (!t->LiveOut->contains(t->op0.var))
        return;

    int r1 = getRegForRead(t->op1.var, 0, t->LiveOut);
    int r0 = getRegForWrite(t->op0.var, r1, 0, t->LiveOut);

    addInstr(op, _reg[r0], _reg[r1], NULL, 0, EMPTY_STR, NULL);
}


void RiscvDesc::emitCallTac(RiscvInstr::OpCode op,Tac *t) {

    //将call指令后面liveout入栈

    spillDirtyRegs(t->LiveOut);
    //Set<Temp>* liveness = t->LiveOut->clone();

    // {
    //     int cnt = 0;
    //     for(auto temp : *liveness){
    //         cnt -= 4;
    //         int r1 = getRegForRead(temp, 0, t->LiveOut);
    //         addInstr(RiscvInstr::SW,  _reg[r1], _reg[RiscvReg::SP], NULL, cnt, EMPTY_STR, NULL);
    //     }
    //     addInstr(RiscvInstr::ADDI, _reg[RiscvReg::SP], _reg[RiscvReg::SP], NULL, cnt, EMPTY_STR, NULL);
    // }
    //参数传递
    // int count = 0,count1=0;
    // Tac *it = t->prev;
    // while (it != NULL)
    // {
    //     if(it->op_code == Tac::PUSH){
    //         count += 4;
    //     }
    //     it = it->prev;
    // }
    
    //for(Tac *it = t->prev; it != NULL && it->op_code == Tac::PUSH; it = it->prev) count += 4,count1++;
    // int count=canlian.size()*4;
    // int cnt = count;
    // addInstr(RiscvInstr::ADDI, _reg[RiscvReg::SP], _reg[RiscvReg::SP], NULL, -count, EMPTY_STR, NULL);

    // for(auto item:canlian){
    //     cnt -= 4;
    //     int r1 = getRegForRead(item.op0.var, 0, item.LiveOut);
    //     addInstr(RiscvInstr::SW,  _reg[r1], _reg[RiscvReg::SP], NULL, cnt, EMPTY_STR, NULL);
    // }
    // canlian.clear();
    // if(count > 0){
        
    //     addInstr(RiscvInstr::ADDI, _reg[RiscvReg::SP], _reg[RiscvReg::SP], NULL, -count, EMPTY_STR, NULL);
    //     int cnt = count;
    //     for(Tac *it = t->prev; it != NULL && it->op_code == Tac::PUSH; it = it->prev){
    //         cnt -= 4;
    //         int r1 = getRegForRead(it->op0.var, 0, it->LiveOut);
    //         addInstr(RiscvInstr::SW,  _reg[r1], _reg[RiscvReg::SP], NULL, cnt, EMPTY_STR, NULL);
    //     }
        // for(int i=0;i<count1;i++){
        //     emitPushTac(t);
        //}
    //}
    //int count += liveness->size() * 4;
    if(t->op1.label->str_form=="getint"||
        t->op1.label->str_form=="getch"||
        t->op1.label->str_form=="getarray"||
        t->op1.label->str_form=="putint"||
        t->op1.label->str_form=="putch"||
        t->op1.label->str_form=="putarray"
    ){
        addInstr(op, NULL, NULL, NULL, 0,  t->op1.label->str_form, NULL);
    }
    else if(t->op1.label->str_form=="starttime"||
        t->op1.label->str_form=="stoptime"
    ){
        addInstr(op, NULL, NULL, NULL, 0,  std::string("_sysy_") +t->op1.label->str_form, NULL);
    }
    else{
        addInstr(op, NULL, NULL, NULL, 0, std::string("_") + t->op1.label->str_form, NULL);
    }
    
    
    // //栈恢复过程
    // {
    //     int cnt = 0;
    //     addInstr(RiscvInstr::ADDI, _reg[RiscvReg::SP], _reg[RiscvReg::SP], NULL, count, EMPTY_STR, NULL);
    //     for(auto temp: *liveness){
    //         cnt -= 4;
    //         int r1 = getRegForWrite(temp, 0, 0, t->LiveOut);
    //         addInstr(RiscvInstr::LW,  _reg[r1], _reg[RiscvReg::SP], NULL, cnt, EMPTY_STR, NULL);
    //     }
    // }
    //将结果送到指定的寄存器
    int r0 = getRegForWrite(t->op0.var, 0, 0, t->LiveOut);
    addInstr(RiscvInstr::MOVE, _reg[r0], _reg[RiscvReg::A0], NULL, 0, EMPTY_STR, NULL);
    
}

//参数入栈
void RiscvDesc::emitPushTac(Tac *t) {
    
    int r1 = getRegForRead(t->op0.var, 0, t->LiveOut);
    addInstr(RiscvInstr::ADDI, _reg[RiscvReg::SP], _reg[RiscvReg::SP], NULL, -4, EMPTY_STR, NULL);
    addInstr(RiscvInstr::SW,  _reg[r1], _reg[RiscvReg::SP], NULL, 0, EMPTY_STR, NULL);
    //canlian.push_front(*t);
}
void RiscvDesc::emitPushTac1(Tac *t) {

    int r1 = getRegForRead(t->op0.var, 0, t->LiveOut);
    // addInstr(RiscvInstr::ADDI, _reg[RiscvReg::SP], _reg[RiscvReg::SP], NULL, -4, EMPTY_STR, NULL);
    // addInstr(RiscvInstr::SW,  _reg[r1], _reg[RiscvReg::SP], NULL, 0, EMPTY_STR, NULL);
    if(_reg[RiscvReg::A0]->dirty)
        addInstr(RiscvInstr::MOVE,  _reg[RiscvReg::A1], _reg[r1], NULL, 0, EMPTY_STR, NULL);
    else
        addInstr(RiscvInstr::MOVE,  _reg[RiscvReg::A0], _reg[r1], NULL, 0, EMPTY_STR, NULL);
}
/* Translates a Unary TAC into Riscv instructions.
 *
 * PARAMETERS:
 *   t     - the Unary TAC
 */
void RiscvDesc::emitUnaryTac(RiscvInstr::OpCode op, Tac *t) {
    // eliminates useless assignments
    if (!t->LiveOut->contains(t->op0.var))
        return;

    int r1 = getRegForRead(t->op1.var, 0, t->LiveOut);
    int r0 = getRegForWrite(t->op0.var, r1, 0, t->LiveOut);

    addInstr(op, _reg[r0], _reg[r1], NULL, 0, EMPTY_STR, NULL);
}

/* Translates a Binary TAC into Riscv instructions.
 *
 * PARAMETERS:
 *   t     - the Binary TAC
 */
void RiscvDesc::emitBinaryTac(RiscvInstr::OpCode op, Tac *t) {
    // eliminates useless assignments
    if (!t->LiveOut->contains(t->op0.var))
        return;

    Set<Temp>* liveness = t->LiveOut->clone();
    liveness->add(t->op1.var);
    liveness->add(t->op2.var);
    int r1 = getRegForRead(t->op1.var, 0, liveness);
    int r2 = getRegForRead(t->op2.var, r1, liveness);
    int r0 = getRegForWrite(t->op0.var, r1, r2, liveness);
    int r3 = getRegForWrite(t->op0.var, r1, r2,liveness);
    switch(op){
        case RiscvInstr::OpCode::LEQ:
            addInstr(RiscvInstr::OpCode::GTR, _reg[r0], _reg[r1], _reg[r2], 0, EMPTY_STR, NULL);
            addInstr(RiscvInstr::OpCode::XORI, _reg[r0], _reg[r0], NULL, 1, EMPTY_STR, NULL);
            break;
        case RiscvInstr::OpCode::GEQ:
            addInstr(RiscvInstr::OpCode::LES, _reg[r0], _reg[r1], _reg[r2], 0, EMPTY_STR, NULL);
            addInstr(RiscvInstr::OpCode::XORI, _reg[r0], _reg[r0], NULL, 1, EMPTY_STR, NULL);
            break;
        case RiscvInstr::OpCode::EQU:
            addInstr(RiscvInstr::OpCode::XOR, _reg[r0], _reg[r1], _reg[r2], 0, EMPTY_STR, NULL);
            addInstr(RiscvInstr::OpCode::SEQZ, _reg[r0], _reg[r0], NULL, 0, EMPTY_STR, NULL);
            break;
        case RiscvInstr::OpCode::NEQ:
            addInstr(RiscvInstr::OpCode::SUB, _reg[r0], _reg[r1], _reg[r2], 0, EMPTY_STR, NULL);
            addInstr(RiscvInstr::OpCode::SNEZ, _reg[r0], _reg[r0], NULL, 0, EMPTY_STR, NULL);
            break;
        case RiscvInstr::OpCode::LAND:
            addInstr(RiscvInstr::OpCode::SNEZ, _reg[r0], _reg[r1], NULL, 0, EMPTY_STR, NULL);
            addInstr(RiscvInstr::OpCode::SNEZ, _reg[r3], _reg[r2], NULL, 0, EMPTY_STR, NULL);
            addInstr(RiscvInstr::OpCode::AND, _reg[r0], _reg[r0], _reg[r3], 0, EMPTY_STR, NULL);
            break;
        case RiscvInstr::OpCode::LOR:
            addInstr(RiscvInstr::OpCode::OR, _reg[r0], _reg[r1], _reg[r2], 0, EMPTY_STR, NULL);
            addInstr(RiscvInstr::OpCode::SNEZ, _reg[r0], _reg[r0], NULL, 0, EMPTY_STR, NULL);
            break;
        default:
            addInstr(op, _reg[r0], _reg[r1], _reg[r2], 0, EMPTY_STR, NULL);
    }
    
}

/* Outputs a single instruction line.
 *
 * PARAMETERS:
 *   label   - label of this line
 *   body    - instruction
 *   comment - comment of this line
 */
void RiscvDesc::emit(std::string label, const char *body, const char *comment) {
    std::ostream &os(*_result);

    if ((NULL != comment) && (label.empty()) && (NULL == body)) {
        os << "                                  # " << comment;

    } else {
        if (!label.empty())
            os << label << std::left << std::setw(40 - label.length()) << ":";
        else if (NULL != body)
            os << "          " << std::left << std::setw(30) << body;

        if (NULL != comment)
            os << "# " << comment;
    }

    os << std::endl;
}

/* Translates a "Functy" object into assembly code and output.
 *
 * PARAMETERS:
 *   f     - the Functy object
 */
void RiscvDesc::emitFuncty(Functy f) {
    
    mind_assert(NULL != f);

    _frame = new RiscvStackFrameManager(-3 * WORD_SIZE);
    
    FlowGraph *g = FlowGraph::makeGraph(f);
    int m=5;
    while(ctrl_kongzhiliu&&m--) g->simplify();        // simple optimization
    g->analyzeLiveness(); // computes LiveOut set of the basic blocks

    for (FlowGraph::iterator it = g->begin(); it != g->end(); ++it) {
        // all variables shared between basic blocks should be reserved
        Set<Temp> *liveout = (*it)->LiveOut;
        for (Set<Temp>::iterator sit = liveout->begin(); sit != liveout->end();
             ++sit) {
            _frame->reserve(*sit);
        }
        (*it)->entry_label = getNewLabel(); // adds entry label of a basic block
    }
    for (FlowGraph::iterator it = g->begin(); it != g->end(); ++it) {
        BasicBlock *b = *it;
        b->analyzeLiveness(); // computes LiveOut set of every TAC
        alloc(b);
        _frame->reset();
        // translates the TAC sequences of this block
        b->instr_chain = prepareSingleChain(b, g);
        
        simplePeephole((RiscvInstr *)b->instr_chain);
        b->mark = 0; // clears the marks (for the next step)
    }
    if (Option::getLevel() == Option::DATAFLOW) {
        std::cout << "Control-flow Graph of " << f->entry << ":" << std::endl;
        g->dump(std::cout);
        // TO STUDENTS: You might not want to get lots of outputs when
        // debugging.
        //              You can enable the following line so that the program
        //              will terminate after the first Functy is done.
        // std::exit(0);
        return;
    }

    mind_assert(!f->entry->str_form.empty()); // this assertion should hold for every Functy
    // outputs the header of a function
    emitProlog(f->entry, _frame->getStackFrameSize());
    // chains up the assembly code of every basic block and output.
    //
    // ``A trace is a sequence of statements that could be consecutively
    //   executed during the execution of the program. It can include
    //   conditional branches.''
    //           -- Modern Compiler Implementation in Java (the ``Tiger Book'')
    for (FlowGraph::iterator it = g->begin(); it != g->end(); ++it)
        emitTrace(*it, g);
}

/* Outputs the leading code of a function.
 *
 * PARAMETERS:
 *   entry_label - the function label
 *   frame_size  - stack-frame size of this function
 * NOTE:
 *   the prolog code is used to save context and establish the stack frame.
 */
void RiscvDesc::emitProlog(Label entry_label, int frame_size) {
    std::ostringstream oss;

    emit(EMPTY_STR, NULL, NULL); // an empty line
    emit(EMPTY_STR, ".text", NULL);
    if (entry_label->str_form == "main") {
        oss << "main";
    } else {
        oss << entry_label;
    }
    emit(oss.str(), NULL, "function entry"); // marks the function entry label
    oss.str("");
    // saves old context
    emit(EMPTY_STR, "sw    ra, -4(sp)", NULL); // saves old frame pointer
    emit(EMPTY_STR, "sw    fp, -8(sp)", NULL); // saves return address
    // establishes new stack frame (new context)
    emit(EMPTY_STR, "mv    fp, sp", NULL);
    oss << "addi  sp, sp, -" << (frame_size + 2 * WORD_SIZE); // 2 WORD's for old $fp and $ra
    emit(EMPTY_STR, oss.str().c_str(), NULL);
}

/* Outputs a single instruction.
 *
 * PARAMETERS:
 *   i     - the instruction to output
 */
void RiscvDesc::emitInstr(RiscvInstr *i) {
    if (i->cancelled)
        return;
    std::ostringstream oss;
    oss << std::left << std::setw(6);

    switch (i->op_code) {
    case RiscvInstr::COMMENT:
        emit(EMPTY_STR, NULL, i->comment);
        return;

    case RiscvInstr::LI:
        oss << "li" << i->r0->name << ", " << i->i;
        break;

    case RiscvInstr::LA:
        oss << "la" << i->r0->name << ", " << i->l;
        break;

    case RiscvInstr::NEG:
        oss << "neg" << i->r0->name << ", " << i->r1->name;
        break;

    case RiscvInstr::NOT:
        oss << "not" << i->r0->name << ", " << i->r1->name;
        break;

    case RiscvInstr::SEQZ:
        oss << "seqz" << i->r0->name << ", " << i->r1->name;
        break;

    case RiscvInstr::SNEZ:
        oss << "snez" << i->r0->name << ", " << i->r1->name;
        break;

    case RiscvInstr::MOVE:
        if(ctrl_qiangduxueruo){
            if(strcmp(i->r0->name,i->r1->name)==0) break;
            else{
                oss << "mv" << i->r0->name << ", " << i->r1->name;
                break;
            }
        }
        else{
            oss << "mv" << i->r0->name << ", " << i->r1->name;
            break;
        }
        

    case RiscvInstr::LW:
        oss << "lw" << i->r0->name << ", " << i->i << "(" << i->r1->name << ")";
        break;

    case RiscvInstr::SW:
        oss << "sw" << i->r0->name << ", " << i->i << "(" << i->r1->name << ")";
        break;
    
    case RiscvInstr::RET:
        oss << "ret";
        break;
    
    case RiscvInstr::ADD:
        if(ctrl_qiangduxueruo){
            if(strcmp(i->r1->name,"0")==0){
                oss << "mv" << i->r0->name << ", " << i->r2->name;
                break;
            }
            else if(strcmp(i->r2->name,"0")==0){
                oss << "mv" << i->r0->name << ", " << i->r1->name;
                break;
            }
            else{
                oss << "add" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
                break;
            }
        }
        else{
            oss << "add" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
            break;
        }
        

    //step2
    case RiscvInstr::MOD:
        oss << "rem" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;

    case RiscvInstr::SUB:
        oss << "sub" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;

    case RiscvInstr::MUL:
        oss << "mul" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;

    case RiscvInstr::DIV:
        oss << "div" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;
    
    case RiscvInstr::BEQZ:
        oss << "beqz" << i->r0->name << ", " << i->l;
        break;

    case RiscvInstr::J:
        oss << "j" << i->l;
        break;

    case RiscvInstr::XORI:
        oss << "xori" << i->r0->name << ", " << i->r1->name << ", " << i->i;
        break;

    case RiscvInstr::XOR:
        oss << "xor" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;

    case RiscvInstr::LES:
        oss << "slt" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;

    case RiscvInstr::GTR:
        oss << "sgt" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;
    
    case RiscvInstr::AND:
        oss << "and" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;

    case RiscvInstr::OR:
        oss << "or" << i->r0->name << ", " << i->r1->name << ", " << i->r2->name;
        break;
    
    case RiscvInstr::ASSIGN:
        oss << "mv" << i->r0->name << ", " << i->r1->name;
        break;
    case RiscvInstr::ADDI:
        oss << "addi" << i->r0->name << ", " << i->r1->name<< ", " << i->i;
        break;
    case RiscvInstr::CALL:
        oss << "call" << i->l;
        break;
    default:
        mind_assert(false); // other instructions not supported
    }

    emit(EMPTY_STR, oss.str().c_str(), i->comment);
}

/* Outputs a "trace" (see also: RiscvDesc::emitFuncty).
 *
 * PARAMETERS:
 *   b     - the leading basic block of this trace
 *   g     - the control-flow graph
 * NOTE:
 *   we just do a simple depth-first search against the CFG
 */
void RiscvDesc::emitTrace(BasicBlock *b, FlowGraph *g) {
    // a trace is a series of consecutive basic blocks
    if (b->mark > 0)
        return;
    b->mark = 1;
    emit(std::string(b->entry_label), NULL, NULL);

    RiscvInstr *i = (RiscvInstr *)b->instr_chain;
    while (NULL != i) {
        emitInstr(i);
        i = i->next;
    }
    switch (b->end_kind) {
    case BasicBlock::BY_JUMP:
        emitTrace(g->getBlock(b->next[0]), g);
        break;

    case BasicBlock::BY_JZERO:
        emitTrace(g->getBlock(b->next[1]), g);
        break;

    case BasicBlock::BY_RETURN:
        break;

    default:
        mind_assert(false); // unreachable
    }
}

/* Appends an instruction line to "_tail". (internal helper function)
 *
 * PARAMETERS:
 *   op_code - operation code
 *   r0      - the first register operand (if any)
 *   r1      - the second register operand (if any)
 *   r2      - the third register operand (if any)
 *   i       - immediate number or offset (if any)
 *   l       - label operand (for LA and jumps)
 *   cmt     - comment of this line
 */
void RiscvDesc::addInstr(RiscvInstr::OpCode op_code, RiscvReg *r0, RiscvReg *r1,
                         RiscvReg *r2, int i, std::string l, const char *cmt) {
    mind_assert(NULL != _tail);

    // we should eliminate all the comments when doing optimization
    if (Option::doOptimize() && (RiscvInstr::COMMENT == op_code))
        return;
    _tail->next = new RiscvInstr();
    _tail = _tail->next;
    _tail->op_code = op_code;
    _tail->r0 = r0;
    _tail->r1 = r1;
    _tail->r2 = r2;
    _tail->i = i;
    _tail->l = l;
    _tail->comment = cmt;
}


/******************** a simple peephole optimizer *********************/

/* Performs a peephole optimization pass to the instruction sequence.
 *
 * PARAMETERS:
 *   iseq  - the instruction sequence to optimize
 */
void RiscvDesc::simplePeephole(RiscvInstr *iseq) {
    // if you are interested in peephole optimization, you can implement here
    // of course, beyond our requirements
//    while(iseq){
//         if(iseq->op_code==RiscvInstr::MOVE){
//             if(iseq->r0==iseq->r1)
//                 iseq->
//         }
//         iseq=iseq->next;
//    }
}

/******************* REGISTER ALLOCATOR ***********************/

/* Acquires a register to read some variable.
 *
 * PARAMETERS:
 *   v      - the variable to read
 *   avoid1 - the register which should not be selected
 *   live   - current liveness set
 * RETURNS:
 *   number of the register containing the content of v
 */

//寄存器分配针对的是基本块内部
int RiscvDesc::getRegForRead(Temp v, int avoid1, LiveSet *live) {
    std::ostringstream oss;
    if(ctrl_jicunqi){
        if(v->reg!=-1&&_reg[v->reg]->var == v){
            // _reg[v->reg]->var = v;
            _reg[v->reg]->var=v;
            return v->reg;
        }
        else{
            // std::cout<<"2222";
            int i;

            i = RiscvReg::A5;
            if(i==avoid1){
                i=RiscvReg::A6;
            }
            if (v->is_offset_fixed) {
                RiscvReg *base = _reg[RiscvReg::FP];
                oss << "load " << v << " from (" << base->name
                    << (v->offset < 0 ? "" : "+") << v->offset << ") into "
                    << _reg[i]->name;
                addInstr(RiscvInstr::LW, _reg[i], base, NULL, v->offset, EMPTY_STR,
                        oss.str().c_str());

            } else {
                oss << "initialize " << v << " with 0";
                addInstr(RiscvInstr::MOVE, _reg[i], _reg[RiscvReg::ZERO], NULL, 0,
                        EMPTY_STR, oss.str().c_str());
            }
            _reg[i]->dirty = false;
            return i;
        }
    }
    else{
        int i = lookupReg(v);

        if (i < 0) {
            // we wil load the content into some register
            i = lookupReg(NULL);

            if (i < 0) {
                i = selectRegToSpill(avoid1, RiscvReg::ZERO, live);
                spillReg(i, live);
            }

            _reg[i]->var = v;

            if (v->is_offset_fixed) {
                RiscvReg *base = _reg[RiscvReg::FP];
                oss << "load " << v << " from (" << base->name
                    << (v->offset < 0 ? "" : "+") << v->offset << ") into "
                    << _reg[i]->name;
                addInstr(RiscvInstr::LW, _reg[i], base, NULL, v->offset, EMPTY_STR,
                        oss.str().c_str());

            } else {
                oss << "initialize " << v << " with 0";
                addInstr(RiscvInstr::MOVE, _reg[i], _reg[RiscvReg::ZERO], NULL, 0,
                        EMPTY_STR, oss.str().c_str());
            }
            _reg[i]->dirty = false;
        }

        return i;
    }
    
}

/* Acquires a register to write some variable.
 *
 * PARAMETERS:
 *   v      - the variable to write
 *   avoid1 - the register which should not be selected
 *   avoid2 - the same as "avoid1"
 *   live   - the current liveness set
 * RETURNS:
 *   number of the register which can be safely written to
 */
//第一个参数，待分配的变量，第二个参数，我不想要那个，第三个参数，我不想要哪个，不想要的真多
//第四个参数，
int RiscvDesc::getRegForWrite(Temp v, int avoid1, int avoid2, LiveSet *live) {
    if(ctrl_jicunqi){
        if(v->reg!=-1){
            _reg[v->reg]->var = v;
            _reg[v->reg]->dirty = true;
            return v->reg;
        }
        else{
            RiscvReg *base = _reg[RiscvReg::FP];
            _reg[RiscvReg::A5]->var=v;
            if (!v->is_offset_fixed) {
                _frame->getSlotToWrite(v, live);
            }
            std::ostringstream oss;
            addInstr(RiscvInstr::SW, _reg[RiscvReg::A5], base, NULL, v->offset, EMPTY_STR,
                 oss.str().c_str());
            // return i;
            return RiscvReg::A5;
        }
    }
    else{
        if (NULL == v || !live->contains(v))
            return RiscvReg::ZERO;

        int i = lookupReg(v);

        if (i < 0) {
            i = lookupReg(NULL);

            if (i < 0) {
                i = selectRegToSpill(avoid1, avoid2, live);
                spillReg(i, live);
            }
            _reg[i]->var = v;
        }

        _reg[i]->dirty = true;

        return i;
    }
    
}

/* Spills a specified register (into memory, i.e. into the stack-frame).
 *
 * PARAMETERS:
 *   i     - number of the register to spill
 *   live  - the current liveness set
 * NOTE:
 *   if the variable contained in $i is no longer alive,
 *   we don't save it into memory.
 */
void RiscvDesc::spillReg(int i, LiveSet *live) {
    std::ostringstream oss;

    Temp v = _reg[i]->var;

    if ((NULL != v) && _reg[i]->dirty && live->contains(v)) {
        RiscvReg *base = _reg[RiscvReg::FP];

        if (!v->is_offset_fixed) {
            _frame->getSlotToWrite(v, live);
        }

        oss << "spill " << v << " from " << _reg[i]->name << " to ("
            << base->name << (v->offset < 0 ? "" : "+") << v->offset << ")";
        addInstr(RiscvInstr::SW, _reg[i], base, NULL, v->offset, EMPTY_STR,
                 oss.str().c_str());
    }

    _reg[i]->var = NULL;
    _reg[i]->dirty = false;
}

/* Spills all dirty (and alive) registers into memory.
 *
 * PARAMETERS:
 *   live  - the current liveness set
 */
void RiscvDesc::spillDirtyRegs(LiveSet *live) {
    int i;
    // 先从前往后遍历看是否有dirty寄存器并且还是活跃的，如果有，立即停止，然后进行spill
    for (i = 0; i < RiscvReg::TOTAL_NUM; ++i) {
        if ((NULL != _reg[i]->var) && _reg[i]->dirty &&
            live->contains(_reg[i]->var))
            break;

        _reg[i]->var = NULL;
        _reg[i]->dirty = false;
    }

    //执行这一步代表有要spill的，所以我们进行遍历，利用spillreg处理
    if (i < RiscvReg::TOTAL_NUM) {
        addInstr(RiscvInstr::COMMENT, NULL, NULL, NULL, 0, EMPTY_STR,
                 "(save modified registers before control flow changes)");

        for (; i < RiscvReg::TOTAL_NUM; ++i)
            spillReg(i, live);
    }
}

/* Looks up a register containing the specified variable.
 *
 * PARAMETERS:
 *   v     - the specified variable
 * RETURNS:
 *   number of the register if found; -1 if not found
 */
int RiscvDesc::lookupReg(tac::Temp v) {
    for (int i = 0; i < RiscvReg::TOTAL_NUM; ++i)
        if (_reg[i]->general && _reg[i]->var == v)
            return i;

    return -1;
}

/* Selects a register to spill into memory (so that it can be released).
 *
 * PARAMETERS:
 *   avoid1 - the register that should not be selected
 *   avoid2 - the same as avoid1
 *   live   - the current liveness set
 * RETURNS:
 *   number of the selected register
 */
int RiscvDesc::selectRegToSpill(int avoid1, int avoid2, LiveSet *live) {
    if(ctrl_jicunqi){
        //先看看有没有和平的方式
        for (int i = 0; i < RiscvReg::TOTAL_NUM; ++i) {
            //遇到general寄存器跳过，函数参数用
            if (!_reg[i]->general)
                continue;
            //如果不是要避开的1和2，并且这个寄存器现在分配的变量活跃性已经没有了，那么就可以返回这个作为要分配的。
            if ((i != avoid1) && (i != avoid2) && !live->contains(_reg[i]->var))
                return i;
        }
        //没有能够分配的方式，我们采用将一个寄存器dirty，然后继续分配，冻肉肱骨头

        for (int i = 0; i < RiscvReg::TOTAL_NUM; ++i) {
            //遇到general寄存器跳过，函数参数用
            if (!_reg[i]->general)
                continue;
            //如果不是要避开的1和2，并且这个寄存器不是dirty的，返回即可
            if ((i != avoid1) && (i != avoid2) && !_reg[i]->dirty)
                return i;
        }

        //最坏的情况，全是dirty，没有活跃的，找最远活跃的寄存器
        do {
            _lastUsedReg = (_lastUsedReg + 1) % RiscvReg::TOTAL_NUM;
        } while ((_lastUsedReg == avoid1) || (_lastUsedReg == avoid2) ||
                !_reg[_lastUsedReg]->general);

        return _lastUsedReg;
    }
    else{
        for (int i = 0; i < RiscvReg::TOTAL_NUM; ++i) {
            //遇到general寄存器跳过，函数参数用
            if (!_reg[i]->general)
                continue;
            //如果不是要避开的1和2，并且这个寄存器现在分配的变量活跃性已经没有了，那么就可以返回这个作为要分配的。
            if ((i != avoid1) && (i != avoid2) && !live->contains(_reg[i]->var))
                return i;
        }
        for (int i = 0; i < RiscvReg::TOTAL_NUM; ++i) {
            //遇到general寄存器跳过，函数参数用
            if (!_reg[i]->general)
                continue;
            //如果不是要避开的1和2，并且这个寄存器不是dirty的，返回即可
            if ((i != avoid1) && (i != avoid2) && !_reg[i]->dirty)
                return i;
        }
        srand((unsigned)time(NULL));
        int j=0; 
        do {
            j=rand()%RiscvReg::TOTAL_NUM;
        } while ((j == avoid1) || (j == avoid2) ||!_reg[j]->general);
        return j;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


void clear() {
	nodes.clear();
	neighbours.clear();
	nodeDeg.clear();
}

void removeNode(Temp n) {
	nodes.erase(n);
	for (Temp m : neighbours[n])
		if (nodes.find(m)!=nodes.end())
			nodeDeg.insert({m, nodeDeg[m] - 1});
}
void removeRandeNode(){
    srand((unsigned)time(NULL));
    int j=rand()%10;
    int x=0;
    Temp n;
    for(auto i=nodes.begin();i!=nodes.end();i++){
        if(x==j){
            n=*i;
            break;
        }
        x++;
    }
    if(x>nodes.size()){
        for(auto i=nodes.begin();i!=nodes.end();i++){
            n=*i;
            break;
        }
    }
    n->reg=-1;
    nodes.erase(n);
	for (Temp m : neighbours[n])
		if (nodes.find(m)!=nodes.end())
			nodeDeg.insert({m, nodeDeg[m] - 1});
}

int chooseAvailableRegister(Temp n) {
	std::set<int> usedRegs;
	for (Temp m : neighbours[n]) {
		if (m->reg == -1) continue;
		usedRegs.insert(m->reg);
	}
	for (int r : regs)
		if (!(usedRegs.find(r)!=usedRegs.end()))
			return r;
	return -1;
}

bool color() {
		if (nodes.empty())
			return true;

	
		Temp n = NULL;
		for (Temp t : nodes) {
			if (nodeDeg[t] < 18) {
				n = t;
				break;
			}
		}

		if (n != NULL) {
			
			removeNode(n);
			bool subColor = color();
			n->reg = chooseAvailableRegister(n);
			return subColor;
		} else {
            removeRandeNode();
			color();
            return true;
		}
	}

void addNode(Temp node) {
	if (nodes.find(node)!=nodes.end()) return;
	//if (node->reg != -1 && node->reg.equals(fp)) return;
	nodes.insert(node);
    std::set<Temp> a;
	neighbours.insert({node,a });
	nodeDeg.insert({node, 0});
}
void addEdge(Temp a, Temp b) {
		neighbours[a].insert(b);
		neighbours[b].insert(a);
		nodeDeg.insert({a, nodeDeg[a] + 1});
		nodeDeg.insert({b, nodeDeg[b] + 1});
	}









void makeNodes(BasicBlock *b) {
        auto c=b->LiveUse;
		
        for(auto i=c->begin();i!=c->end();i++){
            addNode(*i);
        }
        
		for (auto tac = b->tac_chain; tac != NULL; tac = tac->next) {
			switch (tac->op_code) {
				case Tac::ADD: 
                case Tac::SUB: 
                case Tac::MUL: 
                case Tac::DIV: 
                case Tac::MOD:
				case Tac::LAND: 
                case Tac::LOR: 
                case Tac::GTR: 
                case Tac::GEQ:
                case Tac::EQU:
				case Tac::NEQ: 
                case Tac::LEQ: 
                case Tac::LES:
					addNode(tac->op0.var); addNode(tac->op1.var); addNode(tac->op2.var);
					break;

				case Tac::NEG: 
                case Tac::LNOT: 
                case Tac::ASSIGN:
                case Tac::BNOT:
                case Tac::LOAD:
					addNode(tac->op0.var); addNode(tac->op1.var);
					break;


                case Tac::POP:
                case Tac::ALLOC:
                case Tac::LOAD_IMM4:
                case Tac::LOADSYMBOL:
                    addNode(tac->op0.var);
					break;

				case Tac::PUSH:
                case Tac::PUSH1:
                case Tac::CALL: 
					addNode(tac->op0.var);
					break;

	
				case Tac::STORE:
					addNode(tac->op0.var); addNode(tac->op1.var);
					break;

				default:
                    mind_assert(false); 
                                
                    break;
			}
		}
	}



	void makeEdges(BasicBlock *bb) {

		// Ensure that all variables in liveUse have different colors.
		// In fact this is not necessary unless they are loaded at the same time,
		// just as we are required to do, unfortunately.
		for (auto a:*(bb->LiveUse)) {
			for (auto b : *(bb->LiveUse)) {
				if (a!=b && !(neighbours[a].find(b)!=neighbours[a].end())) {
					addEdge(a, b);
				}
			}
		}

		for (auto tac = bb->tac_chain; tac != NULL; tac = tac->next) {
			switch (tac->op_code) {
				case Tac::ADD:
				case Tac::SUB:
				case Tac::MUL:
				case Tac::DIV:
				case Tac::MOD:
				case Tac::LAND:
				case Tac::LOR:
				case Tac::GTR:
				case Tac::GEQ:
				case Tac::EQU:
				case Tac::NEQ:
				case Tac::LEQ:
				case Tac::LES:

				case Tac::NEG:
				case Tac::LNOT:
                case Tac::BNOT:
				case Tac::ASSIGN:
                case Tac::LOAD:
                case Tac::LOAD_IMM4:
                case Tac::LOADSYMBOL:
                case Tac::CALL:
                case Tac::ALLOC:
					if (tac->op0.var != NULL && tac->LiveOut != NULL) {
						for (auto out : *tac->LiveOut) {
							// only consider liveOut inside B
                        
							if (!(out==tac->op0.var) && nodes.find(out)!=nodes.end()){
                                // std::cout<<"1111111111"<<" ";
								addEdge(tac->op0.var, out);
							}
						}
					}
					break;
                case Tac::PUSH:
                case Tac::PUSH1:
                
				case Tac::STORE:
                case Tac::POP:
                
                
					// use op0
					break;

				default:
                    mind_assert(false); 
                                
                    break;
			}
		}
	}

void makeGraph(BasicBlock *b) {
	
	makeNodes(b);
	// for(auto i:nodes){
    //     std::cout<<i<<" ";
    // }
	makeEdges(b);
    // for(auto i:neighbours){
    //     std::cout<<i.first<<"         ";
    //     for(auto j:i.second){
    //         std::cout<<j<<" ";
    //     }
    // }
}

void alloc(BasicBlock *b){
    do{
		clear();
		makeGraph(b);
	} while (!color());
}

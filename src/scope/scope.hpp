/*****************************************************
 *  Definition of Mind Scopes.
 *
 *  This file contains the definition of three kind of scopes:
 *    1. GlobalScope - the global scope
 *    2. FuncScope   - the scope governed by a function definition
 *    3. LocalScope  - a local scope
 *
 *  Keltin Leung 
 */

#ifndef __MIND_SCOPE__
#define __MIND_SCOPE__

#include "define.hpp"
#include <unordered_map>

#include <iostream>

namespace mind {

#define MIND_SCOPE_DEFINED
namespace scope {

/* Scope Iterator.
 * //用来迭代scope中的所有符号
 * This iterator is used to iterate all the Symbol's in the Scope.
 */
class ScopeIterator {
    friend class Scope;

  private:
    // the underlying map iterator type
    typedef std::unordered_map<std::string, symb::Symbol *>::iterator miterator;
    // the first and beyond-last iterators
    miterator _mit, _mit_end;
    // the private constructor for Scope
    ScopeIterator(const miterator &, const miterator &);

  public:
    // the copy constructor
    ScopeIterator(const ScopeIterator &);
    // the equivalence operator
    bool operator==(const ScopeIterator &) const;
    // the inequivalence operator
    bool operator!=(const ScopeIterator &) const;
    // the advance operator
    ScopeIterator &operator++();
    // the post-advance operator
    ScopeIterator operator++(int);
    // the dereference operator
    symb::Symbol *operator*() const;
};

/* Scope (base class of all the concrete scopes).
 *
 * Every scope is actually a symbol table.
 */
class Scope {
  protected:
    // the underlying map
    std::unordered_map<std::string, symb::Symbol *> _syms;

  public:
    // kind of the scopes  作用域(scope)的种类，三种：全局，函数，局部
    typedef enum { GLOBAL, FUNCTION, LOCAL } kind_t;

    // the iterator type 迭代器的内型
    typedef ScopeIterator iterator;
    // Gets the scope kind 得到scope的类型
    virtual kind_t getKind(void) = 0;
    // Tests whether it is a global scope 
    virtual bool isGlobalScope(void);
    // Tests whether it is a function scope  为啥不测试局部，搞歧视？给你一拳
    virtual bool isFuncScope(void);
    // Looks up a name in this scope  寻找一个名字
    virtual symb::Symbol *lookup(std::string, Location *loc);
    // Declares a symbol in this scope  定义一个scope的符号
    virtual void declare(symb::Symbol *);
    // Cancels an already-declared symbol in this scope 删除一个已经定义的符号
    virtual void cancel(symb::Symbol *);
    // Gets the begin iterator  得到初始迭代器
    virtual iterator begin(void);
    // Gets the beyond-last iterator  得到最后一个迭代器
    virtual iterator end(void);
    // Prints the content of this scope 打印这个scope的内容，bugfree是不是可以不写
    virtual void dump(std::ostream &) = 0;

    virtual ~Scope() {}
};

/* Global Scope.
 */
class GlobalScope : public Scope {
  public:
    // Constructor
    GlobalScope();
    // Gets the kind of this scope
    virtual kind_t getKind(void);
    // Declares a symbol in this scope
    virtual void declare(symb::Symbol *);
    // Prints the content of this scope
    virtual void dump(std::ostream &);
};

/* Function Scope.
 *
 * Only Variables (either local variable or formal argument) are here.
 */
class FuncScope : public Scope {
  private:
    // owner of this scope (the Function symbol)
    symb::Function *owner;

  public:
    // Constructor
    FuncScope(symb::Function *owner);
    // Gets the owner of this function scope  funscope特有的一个函数，返回函数
    symb::Function *getOwner(void);
    // Gets the kind of this scope
    virtual kind_t getKind(void);
    // Declares a symbol in this scope
    virtual void declare(symb::Symbol *);
    // Prints the content of this scope
    virtual void dump(std::ostream &);
};

/* Local Scope.
 *
 * Only Variables (either local variable or formal argument) are here.
 */
class LocalScope : public Scope {
  public:
    // Gets the kind of this scope
    virtual kind_t getKind(void);
    // Declares a symbol in this scope
    virtual void declare(symb::Symbol *);
    // Prints the content of this scope
    virtual void dump(std::ostream &);
};
} // namespace scope
} // namespace mind

#endif // __MIND_SCOPE__

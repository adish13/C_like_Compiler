#ifndef AST_H
#define AST_H
#include <bits/stdc++.h>
#include "type.hh"
#define NPOS std::string::npos


enum typeExp{
    EMPTY_ASTNODE,
    SEQ_ASTNODE,
    ASSIGNS_ASTNODE,
    RETURN_ASTNODE,
    IF_ASTNODE,
    WHILE_ASTNODE,
    FOR_ASTNODE,
    PROCCALL_ASTNODE,
    IDENTIFIER_ASTNODE,
    ARRAYREF_ASTNODE,
    MEMBER_ASTNODE,
    ARROW_ASTNODE,
    OP_BINARY_ASTNODE,
    OP_UNARY_ASTNODE,
    ASSIGNE_ASTNODE,
    FUNCALL_ASTNODE,
    INTCONST_ASTNODE,
    FLOATCONST_ASTNODE,
    STRINGCONST_ASTNODE    
};

class abstract_astnode{
    public:
    virtual void print() = 0;
    enum typeExp astnode_type;
    
};

class exp_astnode: public abstract_astnode{
public:
    std::string exp_type;
    bool has_lval;

};

class ref_astnode:public exp_astnode{

};

class identifier_astnode : public ref_astnode{
public:
    std::string identifier;

    identifier_astnode(std::string exp_type,std::string identifier);

    void print();

};

class arrayref_astnode : public ref_astnode{
public:
    exp_astnode *exp1,*exp2;

    arrayref_astnode(std::string exp_type,exp_astnode *exp1, exp_astnode *exp2);
    void print();

};

class member_astnode : public ref_astnode{
public:
    exp_astnode *exp;
    identifier_astnode *id;

    member_astnode(std::string exp_type,exp_astnode *exp, identifier_astnode *id);
    void print();

};

class arrow_astnode : public ref_astnode{
public:
    exp_astnode *exp;
    identifier_astnode *id;

    arrow_astnode(std::string exp_type,exp_astnode *exp, identifier_astnode *id);

    void print();

};

class op_binary_astnode : public exp_astnode{
public:
    std::string obs;
    std::string obs_if;
    exp_astnode* lexp,*rexp;

    op_binary_astnode(std::string exp_type,std::string obs, std::string obs_if, exp_astnode* lexp, exp_astnode *rexp);
    void print();

};

class op_unary_astnode : public exp_astnode{
public:
    std::string ous;
    exp_astnode* cexp;

    op_unary_astnode(std::string exp_type,std::string ous, bool has_lval, exp_astnode *cexp);
    void print();

};

class assignE_astnode : public exp_astnode{
public:
    exp_astnode* lexp,*rexp;

    assignE_astnode(std::string exp_type, exp_astnode* lexp, exp_astnode *rexp);
    void print();

};

class funcall_astnode : public exp_astnode{
public:
    identifier_astnode *func;
    std::vector<exp_astnode *> params;

    funcall_astnode(std::string exp_type,identifier_astnode *func, std::vector<exp_astnode *> params);
    void print();

};

class intconst_astnode : public exp_astnode{
public:
    int val;

    intconst_astnode(std::string exp_type,int val);
    void print();


};

class floatconst_astnode : public exp_astnode{
public:
    float val;

    floatconst_astnode(std::string exp_type,float val);

     void print();

};

class stringconst_astnode : public exp_astnode{
public:
    std::string val;

    stringconst_astnode(std::string exp_type,std::string val);
    void print();

};

class statement_astnode: public abstract_astnode{

};

class empty_astnode: public statement_astnode{
public:

    empty_astnode();
    void print();

};

class seq_astnode: public statement_astnode{
public:
    std::vector<statement_astnode *> statements;
    seq_astnode(std::vector<statement_astnode *> statements);
    void print();

};


class assignS_astnode: public statement_astnode{
public: 
    exp_astnode *lexp,*rexp;
    assignS_astnode(assignE_astnode* ase);
    void print();

};

class return_astnode: public statement_astnode{
public:
    exp_astnode *cexp;
    return_astnode(exp_astnode *cexp);

    void print();

};

class if_astnode: public statement_astnode{
public:
    exp_astnode *lexp;
    statement_astnode *s1,*s2;
    if_astnode(exp_astnode *lexp,statement_astnode* s1,statement_astnode* s2);

     void print();

};

class while_astnode: public statement_astnode{
public:
    exp_astnode *lexp;
    statement_astnode *s1;
    while_astnode(exp_astnode *lexp,statement_astnode* s1);
    void print();


};

class for_astnode: public statement_astnode{
public:
    exp_astnode *lexp,*cexp,*rexp;
    statement_astnode *s;
    for_astnode(exp_astnode *lexp,exp_astnode* cexp,exp_astnode *rexp,statement_astnode *s);
    void print();

};

class proccall_astnode: public statement_astnode{
public:
    identifier_astnode *id;
    std::vector<exp_astnode *> expressions;
    proccall_astnode(identifier_astnode *id, std::vector<exp_astnode *> expressions);

    void print();

};

bool matching_assgn_exps(std::string lt, std::string rt);
assignE_astnode* assignment_type_checking(std::string lt,std::string rt,exp_astnode* e1,exp_astnode* e2,std::string &em);
bool and_or_op_typechecking(std::string lt, std::string rt);
bool pointer_array_typechecking(std::string lt,std::string rt);
std::string dereference_type(std::string type);
op_binary_astnode* type_checking(std::string lt,std::string rt,std::string op,exp_astnode* e1,exp_astnode* e2);
identifier_astnode* array_decay_to_pointer(std::string id, std::string og_type);
std::string argument_type_checking(std::string id,std::map<std::string , std::vector<inherited_type*>> func_params,std::vector<exp_astnode*> &args);
return_astnode* return_typechecking(std::string &em,std::string lt,std::string rt,exp_astnode* exp);

#endif
#include "ast.hh"
extern std::regex arr_regex;
extern std::regex ptr_regex;

void print_2_expressions(std::string title,std::string label1,std::string label2,exp_astnode* e1,exp_astnode* e2){

    std::cout << "{\n";
    std::cout << "\"" + title + "\": {\n";
    std::cout << "\"" + label1 + "\": {\n";
    e1->print();
    std::cout << ",\n";
    std::cout << "\"" + label2 + "\": {\n";
    e2->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

void print_exp_id(std::string title,std::string label1,std::string label2,exp_astnode* e1,identifier_astnode* id){

    std::cout << "{\n";
    std::cout << "\"" + title + "\": {\n";
    std::cout << "\"" + label1 + "\": {\n";
    e1->print();
    std::cout << ",\n";
    std::cout << "\"" + label2 + "\": {\n";
    id->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

identifier_astnode::identifier_astnode(std::string exp_type, std::string identifier)
{
    this->exp_type = exp_type;
    this->has_lval = true;
    this->identifier = identifier;
    this->astnode_type = IDENTIFIER_ASTNODE;

}

void identifier_astnode::print()
{
    ;
    std::cout << "{\n";
    std::cout << "\"identifier\": \"" << this->identifier << "\"\n";
    std::cout << '}';
}

arrayref_astnode ::arrayref_astnode(std::string exp_type, exp_astnode *exp1, exp_astnode *exp2)
{
    this->exp_type = exp_type;
    this->has_lval = true;
    this->exp1 = exp1;
    this->exp2 = exp2;
    this->astnode_type = ARRAYREF_ASTNODE;

}

void arrayref_astnode::print()
{
    ;
    std::cout << "{\n";
    std::cout << "\"arrayref\": {\n";
    std::cout << "\"array\": ";
    this->exp1->print();
    std::cout << ",\n";
    std::cout << "\"index\": ";
    this->exp2->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
    // print_2_expressions("arrayref","array","index",this->exp1,this->exp2);
}

member_astnode::member_astnode(std::string exp_type, exp_astnode *exp, identifier_astnode *id)
{
    this->exp_type = exp_type;
    this->has_lval = exp->has_lval;
    this->exp = exp;
    this->id = id;
    this->astnode_type = MEMBER_ASTNODE;

}

void member_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"member\": {\n";
    std::cout << "\"struct\": ";
    this->exp->print();
    std::cout << ",\n";
    std::cout << "\"field\": ";
    this->id->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
    // print_exp_id("member","struct","field",this->exp,this->id);

}

arrow_astnode::arrow_astnode(std::string exp_type, exp_astnode *exp, identifier_astnode *id)
{
    this->exp_type = exp_type;
    this->has_lval = true;
    this->exp = exp;
    this->id = id;
    this->astnode_type = ARROW_ASTNODE;

}

void arrow_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"arrow\": {\n";
    std::cout << "\"pointer\": ";
    this->exp->print();
    std::cout << ",\n";
    std::cout << "\"field\": ";
    this->id->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';

    // print_exp_id("arrow","pointer","field",this->exp,this->id);

}

op_binary_astnode::op_binary_astnode(std::string exp_type, std::string obs, std::string obs_if, exp_astnode *lexp, exp_astnode *rexp)
{
    this->exp_type = exp_type;
    this->has_lval = false;
    this->obs = obs;
    this->obs_if = obs_if;
    this->lexp = lexp;
    this->rexp = rexp;
    this->astnode_type = OP_BINARY_ASTNODE;

}

void op_binary_astnode::print()
{
    std::string s1 = this->obs;
    std::string s2 = this->obs_if;
    if (s1.back() == '_' && s2.front() == '_')
    {
        s1.pop_back(); // remove the extra underscore from s1
    }
    std::string op = s1 + s2;
    std::cout << "{\n";
    std::cout << "\"op_binary\": {\n";
    std::cout << "\"op\": \"" << op << "\",\n";
    std::cout << "\"left\": ";
    this->lexp->print();
    std::cout << ",\n";
    std::cout << "\"right\": ";
    this->rexp->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';

}

op_unary_astnode::op_unary_astnode(std::string exp_type, std::string ous, bool has_lval, exp_astnode *cexp)
{
    this->exp_type = exp_type;
    this->ous = ous;
    if(ous == "DEREF"){
        this->has_lval = true;
    }
    else{
        this->has_lval = false;
    }
    this->cexp = cexp;
    this->astnode_type = OP_UNARY_ASTNODE;

}

void op_unary_astnode::print()
{
    
    std::string op = (this->ous);
    std::cout << "{\n";
    std::cout << "\"op_unary\": {\n";
    std::cout << "\"op\": \"" << op << "\",\n";
    std::cout << "\"child\": ";
    this->cexp->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

assignE_astnode::assignE_astnode(std::string exp_type, exp_astnode *lexp, exp_astnode *rexp)
{
    this->exp_type = exp_type;
    this->has_lval = false;
    this->lexp = lexp;
    this->rexp = rexp;
    this->astnode_type = ASSIGNE_ASTNODE;

}

void assignE_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"assignE\": {\n";
    std::cout << "\"left\": ";
    this->lexp->print();
    std::cout << ",\n";
    std::cout << "\"right\": ";
    this->rexp->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

funcall_astnode::funcall_astnode(std::string exp_type, identifier_astnode *func, std::vector<exp_astnode *> params)
{
    this->exp_type = exp_type;
    this->has_lval = false;
    this->func = func;
    this->params = params;
    this->astnode_type = FUNCALL_ASTNODE;

}

void funcall_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"funcall\": {\n";
    std::cout << "\"fname\": ";
    this->func->print();
    std::cout << ",\n";
    if (!this->params.size())
    {
        std::cout << "\"params\": []\n";
    }
    else
    {
        std::cout << "\"params\": [\n";

        for (uint i = 0; i < this->params.size(); i++)
        {
            if (i != 0)
            {
                std::cout << ",\n";
            }
            this->params[i]->print();
        }
        std::cout << "\n";
        std::cout << "]\n";
    }
    std::cout << "}\n";
    std::cout << '}';
}

void single_val_print(std::string label,std::string val){
    std::cout << "{\n";
    std::cout << "\"" + label + "\": " << val << "\n";
    std::cout << '}';
}
intconst_astnode::intconst_astnode(std::string exp_type, int val)
{
    this->exp_type = exp_type;
    this->has_lval = false;
    this->val = val;
    this->astnode_type = INTCONST_ASTNODE;

}

void intconst_astnode::print()
{
    std::cout << "{\n";
    std::cout << "\"intconst\": " << this->val << "\n";
    std::cout << '}';
}

floatconst_astnode::floatconst_astnode(std::string exp_type, float val)
{
    this->exp_type = exp_type;
    this->has_lval = 0;
    this->val = val;
    this->astnode_type = FLOATCONST_ASTNODE;

}

void floatconst_astnode::print()
{

    std::cout << "{\n";
    std::cout << "\"floatconst\": " << this->val << "\n";
    std::cout << '}';

}

stringconst_astnode::stringconst_astnode(std::string exp_type, std::string val)
{
    this->exp_type = exp_type;
    this->has_lval = false;
    this->val = val;
    this->astnode_type = STRINGCONST_ASTNODE;

}

void stringconst_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"stringconst\": " << this->val << "\n";
    std::cout << '}';

}

empty_astnode::empty_astnode()
{
    this->astnode_type = EMPTY_ASTNODE;
}

void empty_astnode::print()
{
    std::cout << "\"empty\"";
}

seq_astnode::seq_astnode(std::vector<statement_astnode *> statements)
{
    this->statements = statements;
    this->astnode_type = SEQ_ASTNODE;

}

void seq_astnode::print()
{
    
    std::cout << "{\n";

    if (!this->statements.size())
    {
        std::cout << "\"seq\": []\n";
    }
    else
    {
        std::cout << "\"seq\": [\n";

        for (uint i = 0; i < this->statements.size(); i++)
        {
            if (i != 0)
            {
                std::cout << ",\n";
            }
            this->statements[i]->print();
        }
        std::cout << "\n";
        std::cout << "]\n";
    }
    std::cout << '}';
}

assignS_astnode::assignS_astnode(assignE_astnode *ase)
{
    this->lexp = ase->lexp;
    this->rexp = ase->rexp;
    this->astnode_type = ASSIGNS_ASTNODE;

}

void assignS_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"assignS\": {\n";
    std::cout << "\"left\": ";
    this->lexp->print();
    std::cout << ",\n";
    std::cout << "\"right\": ";
    this->rexp->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

return_astnode::return_astnode(exp_astnode *cexp)
{
    this->cexp = cexp;
    this->astnode_type = RETURN_ASTNODE;

}

void return_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"return\": \n";
    this->cexp->print();
    std::cout << "\n";
    std::cout << '}';
}

if_astnode::if_astnode(exp_astnode *lexp, statement_astnode *s1, statement_astnode *s2)
{
    this->lexp = lexp;
    this->s1 = s1;
    this->s2 = s2;
    this->astnode_type = IF_ASTNODE;

}

void if_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"if\": {\n";
    std::cout << "\"cond\": ";
    this->lexp->print();
    std::cout << ",\n";
    std::cout << "\"then\": ";
    this->s1->print();
    std::cout << ",\n";
    std::cout << "\"else\": ";
    this->s2->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

while_astnode::while_astnode(exp_astnode *lexp, statement_astnode *s1)
{
    this->lexp = lexp;
    this->s1 = s1;
    this->astnode_type = WHILE_ASTNODE;

}
void while_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"while\": {\n";
    std::cout << "\"cond\": ";
    this->lexp->print();
    std::cout << ",\n";
    std::cout << "\"stmt\": ";
    this->s1->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

for_astnode::for_astnode(exp_astnode *lexp, exp_astnode *cexp, exp_astnode *rexp, statement_astnode *s)
{
    this->lexp = lexp;
    this->cexp = cexp;
    this->rexp = rexp;
    this->s = s;
    this->astnode_type = FOR_ASTNODE;

}
void for_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"for\": {\n";
    std::cout << "\"init\": ";
    this->lexp->print();
    std::cout << ",\n";
    std::cout << "\"guard\": ";
    this->cexp->print();
    std::cout << ",\n";
    std::cout << "\"step\": ";
    this->rexp->print();
    std::cout << ",\n";
    std::cout << "\"body\": ";
    this->s->print();
    std::cout << "\n";
    std::cout << "}\n";
    std::cout << '}';
}

proccall_astnode::proccall_astnode(identifier_astnode *id, std::vector<exp_astnode *> expressions)
{
    this->expressions = expressions;
    this->id = id;
    this->astnode_type = PROCCALL_ASTNODE;

}

void proccall_astnode::print()
{
    
    std::cout << "{\n";
    std::cout << "\"proccall\": {\n";
    std::cout << "\"fname\": ";
    this->id->print();
    std::cout << ",\n";
    if (!this->expressions.size())
    {
        std::cout << "\"params\": []\n";
    }
    else
    {
        std::cout << "\"params\": [\n";

        for (uint i = 0; i < this->expressions.size(); i++)
        {
            if (i != 0)
            {
                std::cout << ",\n";
            }
            this->expressions[i]->print();
        }
        std::cout << "\n";
        std::cout << "]\n";
    }
    std::cout << "}\n";
    std::cout << '}';
}

bool and_or_op_typechecking(std::string lt, std::string rt)
{

    if (
        (lt == "int" || lt == "float" || (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS) || std::regex_search(lt, ptr_regex)) &&
        (rt == "int" || rt == "float" || (std::regex_search(rt, arr_regex) && rt.find("(") == NPOS) || std::regex_search(rt, ptr_regex)))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool pointer_array_typechecking(std::string lt, std::string rt)
{

    if (lt == rt){
        if (
            (lt == "int" || lt == "float" || (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS) || std::regex_search(lt, ptr_regex)) &&
            (rt == "int" || rt == "float" || (std::regex_search(rt, arr_regex) && rt.find("(") == NPOS) || std::regex_search(rt, ptr_regex)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // // Sample input strings
    // std::string lt = "int[10][20]";
    // std::string rt = "int[12][20]";
    // Regular expression patterns to match array and pointer types

    // Case 1: both strings are arrays
    if (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS && std::regex_search(rt, arr_regex) && rt.find("(") == NPOS)
    {
        // Remove the first [x] from both strings and replace with (*)
        std::string new_lt = std::regex_replace(lt, arr_regex, "(*)", std::regex_constants::format_first_only);
        std::string new_rt = std::regex_replace(rt, arr_regex, "(*)", std::regex_constants::format_first_only);

        // Compare the updated strings
        return (new_lt == new_rt ? true : false);
    }

    // Case 2: left string is array and right string is pointer
    else if (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS && std::regex_search(rt, ptr_regex))
    {
        // Remove the first [x] from left string and replace with (*)
        std::string new_lt = std::regex_replace(lt, arr_regex, "(*)", std::regex_constants::format_first_only);

        // Replace * with (*) in right string if it doesn't contain (
        std::string new_rt = rt;
        if (rt.find("(") != NPOS)
        {
            new_rt = rt;
        }
        else
        {
            new_rt = rt;
            new_rt.pop_back();
            new_rt += "(*)";
        }

        // Compare the updated strings
        return (new_lt == new_rt ? true : false);
    }

    // Case 3: left string is pointer and right string is array
    else if (std::regex_search(rt, arr_regex) && rt.find("(") == NPOS && std::regex_search(lt, ptr_regex))
    {
        // Remove the first [x] from right string and replace with (*)
        std::string new_rt = std::regex_replace(rt, arr_regex, "(*)", std::regex_constants::format_first_only);

        // Replace * with (*) in right string if it doesn't contain (
        std::string new_lt = lt;
        if (lt.find("(") != NPOS)
        {
            new_lt = lt;
        }
        else
        {
            new_lt.pop_back();
            new_lt += "(*)";
        }

        // Compare the updated strings
        return (new_lt == new_rt ? true : false);
    }

    // Case 4: left string is pointer and right string is pointer
    else if (std::regex_search(rt, ptr_regex) && std::regex_search(lt, ptr_regex))
    {
        // Replace * with (*) in right string if it doesn't contain (
        std::string new_lt = lt;
        if (lt.find("(") != NPOS)
        {
            new_lt = lt;
        }
        else
        {
            new_lt.pop_back();
            new_lt += "(*)";
        }
        // Replace * with (*) in right string if it doesn't contain (
        std::string new_rt = rt;
        if (rt.find("(") != NPOS)
        {
            new_rt = rt;
        }
        else
        {
            new_rt.pop_back();
            new_rt += "(*)";
        }

        // Compare the updated strings
        return (new_lt == new_rt ? true : false);
    }
    else
    {
        return false;
    }

    return 0;
}


op_binary_astnode *type_checking(std::string lt, std::string rt, std::string op, exp_astnode *e1, exp_astnode *e2)
{
    
    if (lt == "int" && rt == "int")
    {
        return new op_binary_astnode("int", op, "_INT", e1, e2);
    }
    else if (lt == "float" && rt == "float")
    {
        return new op_binary_astnode("float", op, "_FLOAT", e1, e2);
    }
    else if (lt == "int" && rt == "float")
    {
        return new op_binary_astnode("float", op, "_FLOAT", new op_unary_astnode("float", "TO_FLOAT", 0, e1), e2);
    }
    else if (lt == "float" && rt == "int")
    {
        return new op_binary_astnode("float", op, "_FLOAT", e1, new op_unary_astnode("float", "TO_FLOAT", 0, e2));
    }
    else if(op == "PLUS_" || op == "MULT_" || op == "DIV_"){
        return nullptr;
    }
    else if (pointer_array_typechecking(lt, rt) == true)
    {
        return new op_binary_astnode("int", op, "_INT", e1, e2);
    }
    else
    {
        return nullptr;
    }
}

bool matching_assgn_exps(std::string lt, std::string rt)
{
    if (lt == "void*" && ((std::regex_search(rt, arr_regex) && rt.find("(") == NPOS) || std::regex_search(rt, ptr_regex)))
    {
        return true;
    }
    if (std::regex_search(lt, ptr_regex) && rt == "void*")
    {
        return true;
    }

    // Case 3: left string is pointer and right string is array
    else if (std::regex_search(rt, arr_regex) && rt.find("(") == NPOS && std::regex_search(lt, ptr_regex))
    {
        // Remove the first [x] from right string and replace with (*)
        std::string new_rt = std::regex_replace(rt, arr_regex, "(*)", std::regex_constants::format_first_only);

        // Replace * with (*) in right string if it doesn't contain (
        std::string new_lt = lt;
        if (lt.find("(") != NPOS)
        {
            new_lt = lt;
        }
        else
        {
            new_lt.pop_back();
            new_lt += "(*)";
        }

        // Compare the updated strings
        return (new_lt == new_rt ? true : false);
    }

    // Case 4: left string is pointer and right string is pointer
    else if (std::regex_search(rt, ptr_regex) && std::regex_search(lt, ptr_regex))
    {
        // Replace * with (*) in right string if it doesn't contain (
        std::string new_lt = lt;
        if (lt.find("(") != NPOS)
        {
            new_lt = lt;
        }
        else
        {
            size_t pos = new_lt.rfind("*");  // Find the last occurrence of *

            if (pos != NPOS) {  // If * was found
                new_lt.replace(pos, 1, "(*)");    // Replace it with (*)
            }
        }
        // Replace * with (*) in right string if it doesn't contain (
        std::string new_rt = rt;
        if (rt.find("(") != NPOS)
        {
            new_rt = rt;
        }
        else
        {
            size_t pos = new_rt.rfind("*");  // Find the last occurrence of *

            if (pos != NPOS) {  // If * was found
                new_rt.replace(pos, 1, "(*)");    // Replace it with (*)
            }
        }

        // Compare the updated strings
        return (new_lt == new_rt ? true : false);
    }
    else
    {
        return false;
    }
}

assignE_astnode* assignment_type_checking(std::string lt,std::string rt,exp_astnode* e1,exp_astnode* e2,std::string &em){
    if(!e1->has_lval){   
        em = "Left operand of assignment should have an lvalue";
        return nullptr;
    } 
    else if (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS){
        em = "Incompatible assignment when assigning to type \"" + lt + "\" from type \"" + rt + "\"";
        return nullptr;
    }
    else if(lt == rt){
        return new assignE_astnode(lt,e1,e2);
    }
    else if(lt == "int" && rt == "float"){
        return new assignE_astnode(lt,e1,new op_unary_astnode("int","TO_INT",0,e2));
    }
    else if(lt == "float" && rt == "int"){
        return new assignE_astnode(lt,e1,new op_unary_astnode("float","TO_FLOAT",0,e2));
    } 
    else if(matching_assgn_exps(lt,rt)){
        return new assignE_astnode(lt,e1,e2);
    }
    else if(std::regex_search(lt, ptr_regex) && e2->astnode_type == INTCONST_ASTNODE && ((intconst_astnode*)e2)->val == 0){
        return new assignE_astnode(lt,e1,e2);
    }
    else{
        em = "Incompatible assignment when assigning to type \"" + lt + "\" from type \"" + rt + "\"";
        return nullptr;
    }
}

std::string dereference_type(std::string t)
{

    if (std::regex_search(t, ptr_regex))
    {
        if (t.find("(*)") == NPOS)
        {
            size_t pos = t.rfind("*");  // Find the position of the last *
            if (pos != NPOS) {
                t.erase(pos, 1);  // Erase the last *
            }
        }
        else
        {
            // t.erase(t.begin() + t.find("("), t.begin() + t.find(")") + 1);
            auto start = t.find("(");
            if (start != std::string::npos) {
                t.erase(start, t.find(")", start) - start + 1);
            }
        }
    }
    else if (std::regex_search(t, arr_regex) && t.find("(") == NPOS)
    {
        // t.erase(t.begin() + t.find("["), t.begin() + t.find("]") + 1);
        auto start = t.find("[");
        if (start != std::string::npos) {
            t.erase(start, t.find("]", start) - start + 1);
        }

    }
    return t;
}

identifier_astnode* array_decay_to_pointer(std::string id, std::string og_type){

    bool cond = (std::regex_search(og_type, arr_regex) && og_type.find("(") == NPOS);
    if(cond){
        std::string new_type = std::regex_replace(og_type, arr_regex, "(*)",std::regex_constants::format_first_only);
        return new identifier_astnode(new_type,id);
    }
    else{
        return new identifier_astnode(og_type,id);
    }
}

std::string argument_type_checking(std::string id,std::map<std::string , std::vector<inherited_type*>> map_f_param,std::vector<exp_astnode*> &args){
    std::string error_message;
    if(map_f_param[id].size() > args.size()){
        error_message = "Procedure/Function \"" + id + "\" called with too few arguments";
        return error_message;
    }
    else if(map_f_param[id].size() < args.size()){
        error_message = "Procedure/Function \"" + id + "\" called with too many arguments";
        return error_message;
    }
    for(uint i = 0; i < args.size(); i++){
        std::string lt = map_f_param[id][i]->type;
        std::string rt = args[i]->exp_type;
        if(map_f_param[id][i]->type == args[i]->exp_type){
            continue;
        }            
        else if(map_f_param[id][i]->type == "int" && args[i]->exp_type == "float"){
            args[i] = new op_unary_astnode("int","TO_INT",0,args[i]);
            continue;
        }
        else if(map_f_param[id][i]->type == "float" && args[i]->exp_type == "int"){
            args[i] = new op_unary_astnode("float","TO_FLOAT",0,args[i]);

            continue;
        } 
        else if((map_f_param[id][i]->type == "void*" && (args[i]->exp_type.find('*') != NPOS || args[i]->exp_type.find('[') != NPOS)) || (args[i]->exp_type == "void*" && (map_f_param[id][i]->type.find('*') != NPOS || map_f_param[id][i]->type.find('[')!=NPOS))){
            continue;
        }
        else if(pointer_array_typechecking(lt,rt)){
            continue;
        }
        else if(map_f_param[id][i]->type.find('*') != NPOS && args[i]->astnode_type == INTCONST_ASTNODE && ((intconst_astnode*)args[i])->val == 0){
            continue;
        }
        else{

            error_message = "Expected \"" + map_f_param[id][i]->type + "\" but argument is of type \"" + args[i]->exp_type + "\"";
            return error_message;
        }

    }
    return "no_error";
}

return_astnode* return_typechecking(std::string &em,std::string lt,std::string rt,exp_astnode* exp){
    if(rt == lt){
        return new return_astnode(exp);
    }
    else if(lt == "int" && rt == "float"){
        return new return_astnode(new op_unary_astnode("int","TO_INT",0,exp));
    }
    else if(rt == "int" && lt == "float"){
        return new return_astnode(new op_unary_astnode("float","TO_FLOAT",0,exp));
    }
    else{
        em = "Incompatible type \"" + rt + "\" returned, expected\"" + lt + "\"";
        return nullptr;
    }
}
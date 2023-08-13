%skeleton "lalr1.cc"
%require  "3.0.1"

%defines 
%define api.namespace {IPL}
%define api.parser.class {Parser}

%define parse.trace

%code requires{
    #include "ast.hh"
    #include "location.hh"
    #include "type.hh"
    #include "symboltable.hh"
    namespace IPL {
        class Scanner;
    }

    // # ifndef YY_NULLPTR
    // #  if defined __cplusplus && 201103L <= __cplusplus
    // #   define YY_NULLPTR nullptr
    // #  else
    // #   define YY_NULLPTR 0
    // #  endif
    // # endif

}

%printer { std::cerr << $$; } STRUCT
%printer { std::cerr << $$; } IDENTIFIER
%printer { std::cerr << $$; } VOID
%printer { std::cerr << $$; } INT
%printer { std::cerr << $$; } FLOAT
%printer { std::cerr << $$; } ELSE
%printer { std::cerr << $$; } WHILE
%printer { std::cerr << $$; } FOR
%printer { std::cerr << $$; } IF
%printer { std::cerr << $$; } RETURN
%printer { std::cerr << $$; } FLOAT_CONSTANT
%printer { std::cerr << $$; } INT_CONSTANT
%printer { std::cerr << $$; } OR_OP
%printer { std::cerr << $$; } AND_OP
%printer { std::cerr << $$; } EQ_OP
%printer { std::cerr << $$; } NE_OP
%printer { std::cerr << $$; } LE_OP
%printer { std::cerr << $$; } GE_OP
%printer { std::cerr << $$; } INC_OP
%printer { std::cerr << $$; } PTR_OP
%printer { std::cerr << $$; } STRING_LITERAL
%printer { std::cerr << $$; } OTHERS



%parse-param { Scanner  &scanner  }
%locations
%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <string>

    #include "scanner.hh"
    #define NPOS std::string::npos

    symbol_table* gst = new symbol_table();
    symbol_table* lst = NULL;
    std::map<std::string, abstract_astnode*> ast;

    std::regex arr_regex("\\[\\d+\\]");
    std::regex ptr_regex("\\(\\*\\)|\\*");
    std::pair<std::string,struct inherited_type*> main_t = {"",NULL};

    int offset = 0;
    struct inherited_type *type = NULL;
    struct inherited_type *var_t = NULL;

    std::map<std::string , std::vector<inherited_type*>> map_f_param;
    #undef yylex
    #define yylex IPL::Parser::scanner.yylex

}




%define api.value.type variant
%define parse.assert

%start translation_unit

%token <std::string> STRUCT
%token <std::string> OR_OP
%token <std::string> FLOAT
%token <std::string> NE_OP
%token <std::string> LE_OP
%token <std::string> GE_OP
%token <std::string> ELSE
%token <std::string> WHILE
%token <std::string> FOR
%token <std::string> IF
%token <std::string> RETURN
%token <std::string> FLOAT_CONSTANT
%token <std::string> INT_CONSTANT
%token <std::string> IDENTIFIER
%token <std::string> EQ_OP
%token <std::string> AND_OP
%token <std::string> INC_OP
%token <std::string> VOID
%token <std::string> INT
%token <std::string> PTR_OP
%token <std::string> STRING_LITERAL
%token <std::string> OTHERS
%token  '<' '>' '*' '/' '-' '!' '.' '+' ',' '(' ')' ':' ';' '{' '}' '[' ']' '=' '&'

%nterm <assignE_astnode*> assignment_expression
%nterm <assignS_astnode*> assignment_statement
%nterm <std::vector<inherited_type*>> parameter_list
%nterm <inherited_type*> fun_declarator parameter_declaration declarator declarator_arr
%nterm <seq_astnode*> compound_statement
%nterm <exp_astnode*> expression unary_expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression primary_expression postfix_expression
%nterm <std::vector<statement_astnode*>> statement_list
%nterm <std::vector<exp_astnode*>> expression_list
%nterm <statement_astnode*> statement selection_statement iteration_statement 
%nterm <proccall_astnode*> procedure_call
%nterm <std::string> unary_operator


%%
translation_unit: 
struct_specifier
{ 
}
| function_definition
{ 
} 
| translation_unit struct_specifier
{ 
}
| translation_unit function_definition
{ 
}
;

// 
struct_specifier:
STRUCT IDENTIFIER 
{

    // struct offset is 0
    offset = 0;
    lst = new symbol_table(); 
    std::string id = $1 + ' ' + $2;
    if(gst->records.find(id) != gst->records.end()){
        error(@$, error_message(id,"nil","previously_defined"));
    }
    main_t = {"struct",new inherited_type($2,id,0)};
}
'{' declaration_list '}' ';'
{
    std::string id = $1 + " " + $2;
    struct st_record* a = new st_record(id,"struct","global",offset,0,"",lst);
    add_to_st(gst,a);
}
;

function_definition:
type_specifier 
{

    // Param offset is 12
    offset = 12;
    main_t = {"fun",var_t};

    // initialise local symbol table
    lst = new symbol_table();

}
fun_declarator 
{
    main_t = {"fun",new inherited_type($3->identifier,main_t.second->type,main_t.second->size)};

    // local offset is 0
    offset = 0; 
} 
compound_statement
{
    std::string id = $3->identifier;
    ast.insert({id,$5});
    struct st_record* a = new st_record(id,"fun","global",0,0,$3->type,lst);
    add_to_st(gst,a);
}
;

type_specifier:

STRUCT IDENTIFIER
{
    std::string id = $1 + " " + $2;
    if(main_t.first == "struct" && main_t.second->type == id){
        var_t = new inherited_type($2,id,0);
    }
    else if(gst->records.find(id) == gst->records.end()){
        error(@$,error_message(id,"nil", "not_defined"));
    }
    else{
        var_t = new inherited_type($2,id,gst->records[id]->size);
    }
}
|   VOID
{
    var_t = new inherited_type("","void",0);
}
|	INT
{
    var_t = new inherited_type("","int",4);
}
|	FLOAT
{
    var_t = new inherited_type("","float",4);
}
;

fun_declarator:

IDENTIFIER '(' ')'
{
    std::string id = $1;
    if(gst->records.find(id) != gst->records.end()){
        error(@$, error_message(id,"nil","previously_defined"));
    }
    else{
        $$ = new inherited_type(id,main_t.second->type,main_t.second->size);
    }
}
| IDENTIFIER '(' parameter_list ')'
{   
    map_f_param[$1] = $3;
    std::string id = $1;
    bool cond = gst->records.find(id) != gst->records.end();
    if(cond){
        error(@$, error_message(id,"nil","previously_defined"));
    }
    else{
        $$ = new inherited_type(id,main_t.second->type,main_t.second->size);
        for(uint i = 0; i < $3.size(); i++){
            std::string id = $3[i]->identifier;
            bool cond1 = lst->records.find(id) != lst->records.end();
            if(cond1){
                error(@$, error_message(id,"nil","previously_defined"));
            }
            else{
                offset-=$3[i]->size;
                struct st_record* a = new st_record(id,"var","param",$3[i]->size,offset,$3[i]->type, NULL);
                add_to_st(lst,a);
            }
        }
    }
}
;

parameter_list:
parameter_declaration
{
    $$.push_back($1);
}
| parameter_list ',' parameter_declaration
{ 
    $$ = $1;
    $$.push_back($3);
} 
;

compound_statement:
'{' '}'
{
    $$ = new seq_astnode(std::vector<statement_astnode*> ());
}
| '{' statement_list '}'
{
    $$ = new seq_astnode($2);
}
| '{' declaration_list '}' 
{
    $$ = new seq_astnode(std::vector<statement_astnode*> ());

}
| '{' declaration_list statement_list '}'
{
    $$ = new seq_astnode($3);

}
;

statement_list:
statement
{
    $$.push_back($1);
}
| statement_list statement
{
    $$ = $1;
    $$.push_back($2);
}
;

statement:
';'
{
    $$ = new empty_astnode();
}

| '{' statement_list '}'
{
    $$ = new seq_astnode($2);
}
| selection_statement
{
    $$ = $1;
}
| iteration_statement
{
    $$ = $1;

}
| assignment_statement
{
    $$ = $1;

}
| procedure_call
{
    $$ = $1;
}

| RETURN expression ';'
{
    std::string lt = main_t.second->type;
    std::string rt = $2->exp_type;

    return_astnode* a;
    std::string em = "";
    a = return_typechecking(em,lt,rt,$2);
    if(a == nullptr){
        error(@$,em);
    }
    $$ = a;
}
;

assignment_expression:
unary_expression '=' expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;

    std::string em = "";
    assignE_astnode* a;
    a = assignment_type_checking(lt,rt,$1,$3,em);
    if(a != nullptr){
        $$ = a;
    }
    else{
        error(@$,em);
    }
}
;

assignment_statement:
assignment_expression ';'
{
    $$ = new assignS_astnode($1);   
}
;

procedure_call:
 IDENTIFIER '(' expression_list ')' ';'
{

    std::string id = $1;

    // function already defined earlier
    bool cond1 = gst->records.find(id) != gst->records.end();

    // recursive function call case
    bool cond2 = (main_t.first == "fun" && main_t.second->identifier == $1);

    bool cond3 = already_defined.find(id) != already_defined.end();
    if( cond1 || cond2){
        
        std::string em = argument_type_checking(id,map_f_param,$3);
        if(em != "no_error"){
            error(@$,em);
        }
        if(cond1){
            $$ = new proccall_astnode(new identifier_astnode(gst->records[id]->type,id),$3);
        }
        else{
            $$ = new proccall_astnode(new identifier_astnode(main_t.second->type,id),$3);
        }
    
    }
    else if(!cond3){
        error(@$, "Procedure \"" + id + "\" not declared");

    }
    else{
        $$ = new proccall_astnode(new identifier_astnode(already_defined.at(id),id),$3);
    }
}
| IDENTIFIER '(' ')' ';'
{
    std::string id = $1;

    // function already defined earlier
    bool cond1 = gst->records.find(id) != gst->records.end();

    // recursive function call case
    bool cond2 = (main_t.first == "fun" && main_t.second->identifier == $1);

    bool cond3 = already_defined.find(id) != already_defined.end();

    if(cond1 || cond2){
        if(map_f_param[id].size() != 0){
            error(@$,"Procedure called with too few arguments");
        }
        else{
            if(cond1){
                $$ = new proccall_astnode(new identifier_astnode(gst->records[id]->type,id),std::vector<exp_astnode*> ());
            }
            else{
                $$ = new proccall_astnode(new identifier_astnode(main_t.second->type,id),std::vector<exp_astnode*> ());
            }
        }
    }
    else if(!cond3){
        error(@$, "Procedure \"" + id + "\" not declared");
    }
    else{
        $$ = new proccall_astnode(new identifier_astnode(already_defined.at(id),id),std::vector<exp_astnode*> ());
    }
}
;

expression:
logical_and_expression
{
    $$ = $1;
}
| expression OR_OP logical_and_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    // lt can be array or pointer
    // same with rt
    if( !(and_or_op_typechecking(lt,rt)) ){
        error(@$,"Invalid operand of ||,  not scalar or pointer");
    }
    else{
        $$ = new op_binary_astnode("int","OR_OP","",$1,$3);
    }
}
;

logical_and_expression:
equality_expression
{
    $$ = $1;
}
| logical_and_expression AND_OP equality_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    // lt can be array or pointer
    // same with rt
    if( !(and_or_op_typechecking(lt,rt)) ){
        error(@$,"Invalid operand of &&,  not scalar or pointer");
    }
    else{
        $$ = new op_binary_astnode("int","AND_OP","",$1,$3);
    }
}
;

equality_expression:
relational_expression
{
    $$ = $1;
}
| equality_expression EQ_OP relational_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    op_binary_astnode* a;
    a = type_checking(lt,rt,"EQ_OP_",$1,$3);
    if(a == nullptr){
            error(@$,binary_op_em("==",lt,rt));
    }
    else{
        $$ = a;
    }

}
| equality_expression NE_OP relational_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    op_binary_astnode* a;
    a = type_checking(lt,rt,"NE_OP_",$1,$3);
    if(a == nullptr){
            error(@$,binary_op_em("!=",lt,rt));
    }
    else{
        $$ = a;
    }
}
;

relational_expression:
additive_expression
{
    $$ = $1;
}
| relational_expression '<' additive_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    op_binary_astnode* a;
    a = type_checking(lt,rt,"LT_OP_",$1,$3);
    if(a == nullptr){
            error(@$,binary_op_em("<",lt,rt));
    }
    else{
        $$ = a;
    }
}

| relational_expression '>' additive_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    op_binary_astnode* a;
    a = type_checking(lt,rt,"GT_OP_",$1,$3);
    if(a == nullptr){
            error(@$,binary_op_em(">",lt,rt));
    }
    else{
        $$ = a;
    }
}
| relational_expression LE_OP additive_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    op_binary_astnode* a;
    a = type_checking(lt,rt,"LE_OP_",$1,$3);
    if(a == nullptr){
            error(@$,binary_op_em("<=",lt,rt));
    }
    else{
        $$ = a;
    }
}
| relational_expression GE_OP additive_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    op_binary_astnode* a;
    a = type_checking(lt,rt,"GE_OP_",$1,$3);
    if(a == nullptr){
            error(@$,binary_op_em(">=",lt,rt));
    }
    else{
        $$ = a;
    }
}
;


additive_expression:
multiplicative_expression
{
    $$ = $1;
}
| additive_expression '+' multiplicative_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;

    op_binary_astnode* a;
    a = type_checking(lt,rt,"PLUS_",$1,$3);
    if(a == nullptr){
         if(lt == "int" && (rt.find('*') != NPOS || rt.find('[') != NPOS)){
            $$ = new op_binary_astnode(rt,"PLUS_","_INT",$1,$3);
        }
        else if(rt == "int" && (lt.find('*') != NPOS || lt.find('[') != NPOS)){
            $$ = new op_binary_astnode(lt,"PLUS_","_INT",$1,$3);
        }
        else{
            error(@$,binary_op_em("+",lt,rt));
        }
    }
    else{
        $$ = a;
    }


}
| additive_expression '-' multiplicative_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;
    bool cond1 = (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS);
    bool cond2 =  std::regex_search(lt, ptr_regex);

    op_binary_astnode* a;
    if((cond1 || cond2) && rt == "int"){
        $$ = new op_binary_astnode("int","MINUS_","_INT",$1,$3);
    }
    else{
        a = type_checking(lt,rt,"MINUS_",$1,$3);
        if(a == nullptr){
            if(rt == "int" && (lt.find('*') != NPOS || lt.find('[') != NPOS)){
                $$ = new op_binary_astnode(lt,"MINUS_","_INT",$1,$3);
            }
            else {
                error(@$,binary_op_em("-",lt,rt));
            }
        }
        else{
            $$ = a;
        }
    }
}
;

// done
// modify deref function thing

unary_expression :
postfix_expression
{
    $$ = $1;
}
|	unary_operator unary_expression
{
    if($1 == "ADDRESS"){
        if($2->has_lval){
            std::string t = $2->exp_type;
            if(t.find('(') != NPOS){
                t.insert(t.find('(') + 1,"*");
            }
            else if(t.find('[') != NPOS){
                t.insert(t.find('['),"(*)");
            }
            else{
                t += "(*)";
            }
            $$ = new op_unary_astnode(t,$1,false,$2);
        }
        else{
            error(@$,"Operand of & should have lvalue");

        }
    }

    else if($1 == "UMINUS"){
        bool cond1 = $2->exp_type == "int";
        bool cond2 = $2->exp_type == "float";
        if( cond1 || cond2){
            $$ = new op_unary_astnode($2->exp_type,$1,false,$2);
        }
        else{
            error(@$,unary_op_em($2->exp_type,"-"));
            // error(@$,"Operand of unary - should be an int or float");
        }

    }

    else if($1 == "NOT"){
        std::string lt = $2->exp_type;
        bool cond1 = lt == "int";
        bool cond2 = lt == "float";
        bool cond3 = (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS);
        bool cond4 =  std::regex_search(lt, ptr_regex);

        if(cond1 || cond2 || cond3 || cond4){
            $$ = new op_unary_astnode("int",$1,false,$2);
        }
        else{
            error(@$,"Operand of unary ! should be an int or float or pointer");
        }
    }

    else if($1 == "DEREF"){
        std::string lt = $2->exp_type;
        bool cond1 = (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS);
        bool cond2 =  std::regex_search(lt, ptr_regex);

        if(cond1 || cond2){
            if(dereference_type(lt) != "void*"){
                $$ = new op_unary_astnode(dereference_type(lt),$1,1,$2);
            }   
            else{
                error(@$,"can't deref void* pointer");
            }
        }
        else{
            error(@$,unary_op_em(lt,"*"));
        }


    }

    else{
        error(@$, "No such unary operator exists");
    }
    

}
;

multiplicative_expression:
unary_expression
{
    $$ = $1;
}
| multiplicative_expression '*' unary_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;

    op_binary_astnode* a;
    a = type_checking(lt,rt,"MULT_",$1,$3);
    if(a == nullptr){
        error(@$,binary_op_em("*",lt,rt));
    }
    else{
        $$ = a;
    }
}
| multiplicative_expression '/' unary_expression
{
    std::string lt = $1->exp_type;
    std::string rt = $3->exp_type;

    op_binary_astnode* a;
    a = type_checking(lt,rt,"DIV_",$1,$3);
    if(a == nullptr){
        error(@$,binary_op_em("/",lt,rt));
    }
    else{
        $$ = a;
    }
}
;

postfix_expression:
primary_expression
{
    $$ = $1;
}
| 	postfix_expression '[' expression ']'
{
    std::string lt = $1->exp_type;
    bool cond1 = (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS);
    bool cond2 =  std::regex_search(lt, ptr_regex);

    if($3->exp_type != "int"){
        error(@$,"Array subscript is not an integer");
    }
    else{
        if($3->exp_type != "int"){
            error(@$,"array subscript is not an integer");
        }
        else{
            if(!cond1 && !cond2){
                error(@$,"subscripted value is neither array nor pointer");
            }
            else if(dereference_type($1->exp_type) == "void*"){
                error(@$,"dereferencing void* pointer");
            }
            else{
                $$ = new arrayref_astnode(dereference_type($1->exp_type),$1,$3);
            }
        }
    }
}
| IDENTIFIER '(' ')'
{
    std::string id = $1;

    // function already defined earlier
    bool cond1 = gst->records.find(id) != gst->records.end();

    // recursive function call case
    bool cond2 = (main_t.first == "fun" && main_t.second->identifier == $1);

    bool cond3 = already_defined.find(id) != already_defined.end();

    if(cond1 || cond2){
        if(map_f_param[id].size() != 0){
            error(@$,"Procedure called with too few arguments");
        }
        else{
            if(cond1){
                $$ = new funcall_astnode(gst->records[id]->type,new identifier_astnode(gst->records[id]->type,id),std::vector<exp_astnode*> ());
            }
            else{
                $$ = new funcall_astnode(main_t.second->type,new identifier_astnode(main_t.second->type,id),std::vector<exp_astnode*> ());
            }
        }
    }
    else if(!cond3){
        error(@$, "Procedure \"" + id + "\" not declared");
    }
    else{
        $$ = new funcall_astnode(already_defined.at(id),new identifier_astnode(already_defined.at(id),id),std::vector<exp_astnode*> ());
    }
}
| IDENTIFIER '(' expression_list ')'
{
    std::string id = $1;

    // function already defined earlier
    bool cond1 = gst->records.find(id) != gst->records.end();

    // recursive function call case
    bool cond2 = (main_t.first == "fun" && main_t.second->identifier == $1);

    bool cond3 = already_defined.find(id) != already_defined.end();
    if( cond1 || cond2){
        
        std::string em = argument_type_checking(id,map_f_param,$3);
        if(em != "no_error"){
            error(@$,em);
        }
        if(cond1){
            $$ = new funcall_astnode(gst->records[id]->type,new identifier_astnode(gst->records[id]->type,id),$3);
        }
        else{
            $$ = new funcall_astnode(main_t.second->type,new identifier_astnode(main_t.second->type,id),$3);
        }
    
    }
    else if(!cond3){
        error(@$, "Function \"" + id + "\" not declared");

    }
    else{
        $$ = new funcall_astnode(already_defined.at(id),new identifier_astnode(already_defined.at(id),id),$3);
    }
}

| postfix_expression '.' IDENTIFIER
{
   std::string lt = $1->exp_type;
   bool cond1 = (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS);
   bool cond2 =  std::regex_search(lt, ptr_regex);

    if((lt.find("struct") != 0)){
        error(@$,"Left operand of \".\" is not a structure");
    }
    else if(cond1|| cond2){
        error(@$,"Left operand of \".\" is a pointer");
    }
    symbol_table *a = gst->records[lt]->pointer_to_lst;
    if(a->records.find($3) == a->records.end()){
        error(@$,"struct \"" + lt + "\" has no member named \"" + $3 + '"');
    }
    else{
        $$ = new member_astnode(a->records[$3]->type,$1,new identifier_astnode(a->records[$3]->type,$3));
    }
}

| postfix_expression PTR_OP IDENTIFIER
{
    std::string lt = $1->exp_type;
    bool cond1 = (std::regex_search(lt, arr_regex) && lt.find("(") == NPOS);
    bool cond2 =  std::regex_search(lt, ptr_regex);
    bool cond3 = (std::regex_search(dereference_type(lt), arr_regex) && dereference_type(lt).find("(") == NPOS);
    bool cond4 = (std::regex_search(dereference_type(lt), arr_regex) && dereference_type(lt).find("(") == NPOS);

    if((lt.find("struct") != 0)){
        error(@$,struct_em());
    }
    else if(!cond1 && !cond2){
        error(@$,struct_em());

    }
    else if(cond3 || cond4){
        error(@$,struct_em());

    }
    else{
        std::string temp_lt = lt;
        temp_lt = dereference_type(lt);
        symbol_table *a = gst->records[temp_lt]->pointer_to_lst;
        if(a->records.find($3) == a->records.end()){
            error(@$,"struct \"" + temp_lt + "\" has no member named \"" + $3 + "\"");
        }
        else{
            $$ = new arrow_astnode(a->records[$3]->type,$1,new identifier_astnode(a->records[$3]->type,$3));
        }

    }
}

| postfix_expression INC_OP 
{
    if($1->has_lval){
        std::string lt = $1->exp_type;
        bool cond1 = lt == "int";
        bool cond2 = lt == "float";
        bool cond3 = std::regex_search(lt, ptr_regex);

        if(cond1 || cond2 || cond3){
            $$ = new op_unary_astnode(lt,"PP",false,$1);
        }
        else{
            error(@$,"Operand of \"++\" should be a int,float or pointer");
        }
    }
    else{
        error(@$,"Operand of \"++\" should have lvalue");

    }

}
;

primary_expression:
IDENTIFIER
{

    std::string id = $1;
    if(lst->records.find(id) != lst->records.end()){
        std::string lt = lst->records[id]->type;
        bool cond1 = lst->records[id]->local_global == "param";
        if(cond1){
            identifier_astnode* a;
            a = array_decay_to_pointer(id,lt);
            $$ = a;
        }
        $$ = new identifier_astnode(lt,id);
    }
    else{
        error(@$,"Variable \"" + id + "\" not declared"); 
    }
}
|	INT_CONSTANT
{
    $$ = new intconst_astnode("int",stoi($1));

}
|	FLOAT_CONSTANT
{
    $$ = new floatconst_astnode("float",stof($1));

}
|	STRING_LITERAL
{
    $$ = new stringconst_astnode("string",$1);
}
|	'(' expression ')'
{
    $$ = $2;

}
;

expression_list:
expression
{ 
    $$.push_back($1);
}
| expression_list ',' expression
{ 
    $$ = $1;
    $$.push_back($3);
}
;

unary_operator:
	'&'
{ 
    $$ = "ADDRESS";
} 
|	'!'
{ 
    $$ = "NOT";
} 
|	'*'
{ 
    $$ = "DEREF";
} 
| '-'
{ 
    $$ = "UMINUS";
} 
;

selection_statement:
IF '(' expression ')' statement ELSE statement
{
    $$ = new if_astnode($3,$5,$7);
}
;

iteration_statement:
WHILE '(' expression ')' statement
{
    $$ = new while_astnode($3,$5);
}
|	FOR '(' assignment_expression ';' expression ';' assignment_expression ')' statement
{
    $$ = new for_astnode($3,$5,$7,$9);
}

;

declaration_list: 
declaration
{ 
} 
| declaration_list declaration
{
    
}
;

declaration: 
type_specifier declarator_list ';'
{ 
} 
;

// for declarations, add the variables into the local symbol table
// increase the offsets according to scope (function or struct)
// type action defined after declaration(prev grammar rule) gives issues
declarator_list: 
{
    type = new inherited_type(var_t->identifier,var_t->type,var_t->size);
}
declarator
{ 
    handle_offset(lst,offset,main_t.first,$2);

} 

| declarator_list ',' 
{type = new inherited_type(var_t->identifier,var_t->type,var_t->size);} declarator
{ 
    handle_offset(lst,offset,main_t.first,$4);
} 
;

declarator_arr:
IDENTIFIER
{
    std::string var_t = type->type;

    // variable type shouldn't be void

    if(var_t == "void"){
        error(@$,"Variable of type void can't be declared");
    }

    // in that case, the body shouldn't have a variable of the same struct type 
    // unless it's a pointer, hence checking if size is 0 which means it isn't a pointer
    // e.g : the following program should be invalid
    // struct a{
    //      struct a next;
    // };

    else if(main_t.first == "struct" && type->size == 0){
    //    error(@$,"\"" + type->type + "\" is not defined");
        error(@$,error_message(var_t,"nil", "not_defined"));

    }
    // else check if the variable has already been added to lst 
    else if(lst->records.find($1) != lst->records.end()){
    //    error(@$,"\"" + $1 + "\" has a previous declaration");
        error(@$,error_message(var_t,"nil", "previously_defined"));
    }

    // create a new declaration struct for this variable
    else{
        $$ = new inherited_type($1,type->type,type->size);
    }   
}
| declarator_arr '[' INT_CONSTANT ']'
{

    // change the type to array type
    // change the size of the object to int_const times current size

    $$ = $1;
    $$->size *= stoi($3);
    $$->type += "["+ $3 + "]";
    
}
;

parameter_declaration:
type_specifier 
{
    type = new inherited_type(var_t->identifier,var_t->type,var_t->size);
} 
declarator
{ 
    offset += $3->size;
    $$ = $3;
} 
;

declarator:
declarator_arr
{   
    $$ = $1;
}
| '*' 
{
    // address size is 4
    type->size = 4;
    type->type += "*"; 

} 
declarator
{
    $$ = $3;
}
;


    %%
void IPL::Parser::error( const location_type &l, const std::string &err_message )
{
    std::cout << "Error at line " << l.begin.line << ":"  << err_message << "\n";
    exit(1);
}



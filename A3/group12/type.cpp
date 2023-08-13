#include "type.hh"

inherited_type::inherited_type(std::string identifier,std::string type,int size){
    this->identifier = identifier;
    this->type = type;
    this->size = size;
}

std::string error_message(std::string id,std::string operation_type, std::string message_type){
    std::string error_message;
    if(message_type == "previously_defined"){
        error_message = "\"" + id + "\" has a previous definition/declaration";
    }
    else if(message_type == "not_defined"){
        error_message = "\"" + id + "\" is not defined";
    }
    return error_message;
}

std::string binary_op_em(std::string op,std::string lt,std::string rt){
    std::string error_message;
    error_message = "Invalid operands types for binary " + op + ", \"" + lt + "\" and \"" + rt + "\"";
    return error_message;
}

std::string unary_op_em(std::string op,std::string t){
    std::string error_message;
    error_message = "Invalid operand type \"" + t + "\" of unary " + op;
    return error_message;
}

std::string struct_em(){
    std::string error_message;
    error_message = "Left operand of \"->\" is not a pointer to structure";
    return error_message;
}
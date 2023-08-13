#ifndef TYPE_H
#define TYPE_H
#include <bits/stdc++.h>


const std::map<std::string,std::string> already_defined{{"printf","void"},{"scanf","void"}};

// struct defined for storing type/declaration
// as an inherited attibute
// can be transferred(inherited) to the corresponding identifier
struct inherited_type{
	std::string identifier;
	std::string type;
	int size;
	inherited_type(std::string identifier, std::string type,int size);

};

// some functions for handling common error messages
std::string error_message(std::string id,std::string operation_type, std::string message_type);
std::string binary_op_em(std::string op,std::string lt,std::string rt);
std::string unary_op_em(std::string op,std::string t);
std::string struct_em();

#endif
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <bits/stdc++.h>
#include "type.hh"

class symbol_table;

// struct for a single record entry in the symbol table
// itself has a pointer to it's own symbol table class
struct st_record{

	int size;
	int offset;
	std::string type;
	std::string identifier;
	std::string varfun;
	std::string local_global;
	symbol_table *pointer_to_lst;
	st_record(std::string identifier,std::string varfun,std::string local_global,int size,int offset,std::string type,symbol_table* pointer_to_lst);

};

class symbol_table
{
public:
	std::map<std::string,st_record *> records;
	void print();
};

void add_to_st(symbol_table *st,struct st_record* new_entry);
void handle_offset(symbol_table* st,int &offset,std::string main_type,inherited_type* it);
inherited_type* handle_param_declaration(int &offset,std::string &em,std::string id,std::vector<inherited_type*> params, symbol_table* gst, symbol_table* lst,inherited_type* main_t);
#endif
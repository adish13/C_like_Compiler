#include "symboltable.hh"

st_record::st_record(std::string identifier,std::string varfun,std::string local_global,int size,int offset,std::string type,symbol_table* pointer_to_lst){

    this->offset = offset;
    this->type = type;
    this->pointer_to_lst = pointer_to_lst;
    this->identifier = identifier;
    this->varfun = varfun;
    this->local_global = local_global;
    this->size = size;

}

void symbol_table::print(){
    using namespace std;

    if(!this->records.size()){
        cout << "[]\n";
    }
    else{
        cout << "[\n";
        for(auto it = this->records.begin();it != this->records.end(); it++){
            if(it != this->records.begin()){
                cout << ",\n";
            }
            cout  << "[\n";
            cout << "\"" << it->second->identifier << "\",\n";
            cout << "\"" << it->second->varfun << "\",\n";
            cout << "\"" << it->second->local_global << "\",\n";
            cout << it->second->size << ",\n";
            if(it->second->varfun == "struct"){
                cout  <<"\"-\",\n";
                cout  << "\"-\"\n";
            }
            else{
                cout  << it->second->offset << ",\n";
                cout  << "\"" << it->second->type << "\"\n";
            }
            cout  << "]";
        }
        cout << "\n]";
    }
}	

void add_to_st(symbol_table *st,struct st_record* new_entry){
	st->records.insert({new_entry->identifier,new_entry});
};

// handle offset accordingly
// take into account the main type is fun or struct 
void handle_offset(symbol_table* st,int &offset,std::string main_type,inherited_type* it){
    if(main_type == "struct"){
        std::string id = it->identifier;
        struct st_record* a = new st_record(id,"var","local",it->size,offset,it->type,NULL);
        add_to_st(st,a);
    }
    offset += it->size;
    if(main_type == "fun"){
        std::string id = it->identifier;
        struct st_record* a = new st_record(id,"var","local",it->size,-offset,it->type,NULL);
        add_to_st(st,a);

    }
}

inherited_type* handle_param_declaration(int &offset,std::string &em,std::string id,std::vector<inherited_type*> params, symbol_table* gst, symbol_table* lst,inherited_type* main_t){
    if(gst->records.find(id) != gst->records.end()){
        em = error_message(id,"nil","previously_defined");
        return nullptr;
    }
    else{
        for(uint i = 0; i < params.size(); i++){
            std::string id = params[i]->identifier;
            if(lst->records.find(id) != lst->records.end()){
                em = error_message(id,"nil","previously_defined");
            }
            else{
                offset -= params[i]->size;
                struct st_record* a = new st_record(id,"var","param",params[i]->size,offset,params[i]->type, NULL);
                add_to_st(lst,a);
                return nullptr;
            }
        }
        return new inherited_type(id,main_t->type,main_t->size);

    }
}
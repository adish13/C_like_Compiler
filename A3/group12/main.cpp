#include <cstddef>
#include <cstring>
#include <istream>
#include <iostream>
#include <fstream>
#include "scanner.hh"
#include "parser.tab.hh"
using namespace std;

extern map<string, abstract_astnode *> ast;
extern symbol_table *gst;

int main(int argc, char **argv)
{
	using namespace std;
	fstream in_file, out_file;
	in_file.open(argv[1], ios::in);

	IPL::Scanner scanner(in_file);
	IPL::Parser parser(scanner);

#ifdef YYDEBUG
	parser.set_debug_level(1);
#endif
	parser.parse();
	// create gstfun with function entries only
	symbol_table *gstStructs = new symbol_table(), *gstFuncs = new symbol_table();

	for (const auto entry : gst->records)
	{
		if (entry.second->varfun == "fun")
			gstFuncs->records.insert(entry);

		if (entry.second->varfun == "struct")
			gstStructs->records.insert(entry);
	}
	// start the JSON printing

	cout << "{\n";
	cout << "\"globalST\": ";
	gst->print();
	cout << "," << endl;
	if (gstStructs->records.size())
	{
		cout << "  \"structs\": [" << endl;
		for (auto it = gstStructs->records.begin(); it != gstStructs->records.end(); ++it)

		{
			if (it != gstStructs->records.begin())
			{
				cout << ",\n";
			}
			cout << "{" << endl;
			cout << "\"name\": "
				 << "\"" << it->first << "\"," << endl;
			cout << "\"localST\": " << endl;
			it->second->pointer_to_lst->print();
			cout << "\n"
				 << endl;
			cout << "}";
		}
		cout << "\n";
		cout << "],\n";
	}
	else
	{
		cout << "\"structs\": [],\n";
	}

	if (gstFuncs->records.size())
	{
		cout << "\"functions\": [\n";
		for (auto it = gstFuncs->records.begin(); it != gstFuncs->records.end(); ++it)

		{
			if (it != gstFuncs->records.begin())
			{
				cout << ",\n";
			}
			cout << "{" << endl;
			cout << "\"name\": "
				 << "\"" << it->first << "\"," << endl;
			cout << "\"localST\": " << endl;
			it->second->pointer_to_lst->print();
			cout << "," << endl;
			cout << "\"ast\": " << endl;
			ast[it->first]->print();
			cout << endl;
			cout << "\n"
				 << endl;
			cout << "}";
		}
		cout << "\n";
		cout << "]\n";
	}
	else
	{
		cout << "\"functions\": [],\n";
	}
	cout << "}" << endl;

	fclose(stdout);
}

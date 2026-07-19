#pragma once

#include <parse/factory.h>
#include "include.h"
#include "function.h"
#include "datatype.h"

namespace parse_ucs
{

struct source : parse::syntax
{
	source();
	source(tokenizer &tokens, std::any data=std::any());
	~source();

	string name;
	vector<include> incl;
	vector<datatype> types;
	vector<function> funcs;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens, const parse::registry *registry);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "include.h"
#include "function.h"
#include "datatype.h"

namespace parse_ucs
{

struct source : parse::syntax
{
	source();
	source(tokenizer &tokens, void *data = NULL);
	~source();

	string name;
	vector<include> incl;
	vector<datatype> types;
	vector<function> funcs;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_expression/expression.h>
#include "declaration.h"

namespace parse_ucs {

struct prototype : parse::syntax
{
	prototype();
	prototype(tokenizer &tokens, void *data = NULL);
	~prototype();

	string name;
	vector<declaration> args;
	string ret;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_expression/expression.h>

namespace parse_ucs {

struct declaration : parse::syntax
{
	declaration();
	declaration(tokenizer &tokens, void *data = NULL);
	~declaration();

	string type;
	vector<string> name;
	vector<parse_expression::expression> reset;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

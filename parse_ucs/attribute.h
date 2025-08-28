#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_ucs {

struct attribute : parse::syntax
{
	attribute();
	attribute(tokenizer &tokens, void *data = NULL);
	~attribute();

	string name;
	string value;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_ucs {

struct type_name : parse::syntax {
	type_name();
	type_name(tokenizer &tokens, void *data = NULL);
	~type_name();

	vector<string> names;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_ucs {

struct include : parse::syntax
{
	include();
	include(tokenizer &tokens, void *data = NULL);
	~include();

	vector<string> path;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

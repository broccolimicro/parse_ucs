#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_ucs {

struct require : parse::syntax
{
	require();
	require(tokenizer &tokens, std::any data=std::any());
	~require();

	// path, version
	vector<pair<string, string> > path;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

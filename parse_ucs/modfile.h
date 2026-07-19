#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "require.h"
#include "attribute.h"

namespace parse_ucs
{

struct modfile : parse::syntax {
	modfile();
	modfile(tokenizer &tokens, std::any data=std::any());
	~modfile();

	vector<require> deps;
	vector<attribute> attrs;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

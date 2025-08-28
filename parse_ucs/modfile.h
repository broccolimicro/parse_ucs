#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "require.h"
#include "attribute.h"

namespace parse_ucs
{

struct modfile : parse::syntax {
	modfile();
	modfile(tokenizer &tokens, void *data = NULL);
	~modfile();

	vector<require> deps;
	vector<attribute> attrs;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

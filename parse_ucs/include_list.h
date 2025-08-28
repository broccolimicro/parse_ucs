#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "include.h"

namespace parse_ucs
{

struct include_list : parse::syntax
{
	include_list();
	include_list(tokenizer &tokens, void *data = NULL);
	~include_list();

	vector<include> incl;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "include.h"

namespace parse_ucs
{

struct include_list : parse::syntax
{
	include_list();
	include_list(tokenizer &tokens, std::any data=std::any());
	~include_list();

	vector<include> incl;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

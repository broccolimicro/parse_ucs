#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include "declaration.h"
#include "type_name.h"
#include "expression.h"

namespace parse_ucs {

struct function_decl : parse::syntax {
	using declaration=parse_ucs::declaration_t<expression>;

	function_decl();
	function_decl(tokenizer &tokens, void *data = NULL);
	~function_decl();

	string name;
	string recv;
	vector<declaration> args;
	type_name ret;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_expression/expression.h>
#include "declaration.h"
#include "type_name.h"
#include "expression.h"

namespace parse_ucs {

struct type_signature : parse::syntax {
	type_signature();
	type_signature(tokenizer &tokens, std::any data=std::any());
	~type_signature();

	type_name type;
	vector<expression> size;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

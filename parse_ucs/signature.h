#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_expression/expression.h>
#include "type_name.h"
#include "type_signature.h"
#include "expression.h"

namespace parse_ucs {

struct signature : parse::syntax {
	signature();
	signature(tokenizer &tokens, void *data = NULL);
	~signature();

	type_name recv;
	string name;
	vector<type_signature> args;
	bool unqualified;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

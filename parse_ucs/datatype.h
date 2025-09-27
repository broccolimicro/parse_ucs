#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "declaration.h"
#include "prototype.h"
#include "expression.h"

namespace parse_ucs {

struct datatype : parse::syntax {
	using declaration=parse_ucs::declaration_t<expression>;

	datatype();
	datatype(tokenizer &tokens, void *data = NULL);
	~datatype();
	
	bool isInterface;
	string name;
	vector<declaration> members;
	vector<prototype> protocols;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

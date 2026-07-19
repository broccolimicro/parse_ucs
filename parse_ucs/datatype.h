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
	datatype(tokenizer &tokens, std::any data=std::any());
	~datatype();

	// type, interface, or encoding	
	string kind;
	string name;
	vector<declaration> members;
	vector<prototype> protocols;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

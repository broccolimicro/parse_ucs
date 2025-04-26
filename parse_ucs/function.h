#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include "declaration.h"
#include "type_name.h"

namespace parse_ucs
{

struct language
{
	language();
	language(parse::syntax *(*factory)(tokenizer&, void*), void (*expect)(tokenizer&), void (*register_syntax)(tokenizer&));
	~language();

	parse::syntax *(*factory)(tokenizer&, void*);
	void (*expect)(tokenizer&);
	void (*register_syntax)(tokenizer&);
};

// This represent **desired behaviors**
// 1. Behavioral
// 2. Functional
struct function : parse::syntax
{
	function();
	function(const function &cpy);
	function(tokenizer &tokens, void *data = NULL);
	~function();

	string lang;
	string name;
	string recv;
	vector<declaration> args;
	type_name ret;

	string impl;

	parse::syntax *body;

	static map<string, language> registry;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

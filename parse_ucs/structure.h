#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "declaration.h"

namespace parse_ucs
{

// This represents **circuit structures**
// 1. Wires
// 2. Devices
// 3. Modules
struct structure : parse::syntax
{
	structure();
	structure(tokenizer &tokens, void *data = NULL);
	~structure();

	string name;
	vector<declaration> members;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

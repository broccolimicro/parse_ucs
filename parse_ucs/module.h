/*
 * module.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include "behavior.h"

#ifndef parse_ucs_module_h
#define parse_ucs_module_h

namespace parse_ucs
{
struct module : parse::syntax
{
	module();
	module(tokenizer &tokens, void *data = NULL);
	~module();

	string name;
	vector<behavior> behaviors;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

#endif

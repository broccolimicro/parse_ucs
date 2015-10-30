/*
 * behavior.h
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include "member_name.h"
#include "language.h"

#ifndef parse_ucs_behavior_h
#define parse_ucs_behavior_h


namespace parse_ucs
{

struct behavior : parse::syntax
{
	behavior();
	behavior(const behavior &cpy);
	behavior(tokenizer &tokens, void *data = NULL);
	~behavior();

	string lang;
	string name;
	parse::syntax *content;

	static map<string, language> registry;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

#endif

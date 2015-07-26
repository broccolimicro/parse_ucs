/*
 * type.h
 *
 *  Created on: Jun 26, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>
#include "type_name.h"

#ifndef parse_ucs_type_h
#define parse_ucs_type_h

namespace parse_ucs
{
struct sub_type;

struct type : parse::syntax
{
	type();
	type(tokenizer &tokens, int level = 1, void *data = NULL);
	~type();

	void init();

	vector<sub_type> names;
	int level;

	static vector<string> precedence;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	string to_string(int prev_level, string tab = "") const;
	parse::syntax *clone() const;
};

struct sub_type
{
	sub_type();
	sub_type(type sub);
	sub_type(type_name name);
	~sub_type();

	type sub;
	type_name name;

	string to_string(int level, string tab) const;
};

}

#endif

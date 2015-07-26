/*
 * type_name.h
 *
 *  Created on: Jun 26, 2015
 *      Author: nbingham
 */

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_ucs_type_name_h
#define parse_ucs_type_name_h

namespace parse_ucs
{

struct type_name : parse::syntax
{
	type_name();
	type_name(tokenizer &tokens, void *data = NULL);
	~type_name();

	string name;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif

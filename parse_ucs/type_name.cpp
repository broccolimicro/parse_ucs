/*
 * type_name.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "type_name.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>

namespace parse_ucs
{
type_name::type_name()
{
	debug_name = "type_name";
}

type_name::type_name(tokenizer &tokens, void *data)
{
	debug_name = "type_name";
	parse(tokens, data);
}

type_name::~type_name()
{

}

void type_name::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<parse::instance>();

	if (tokens.decrement(__FILE__, __LINE__, data))
		name = tokens.next();

	tokens.syntax_end(this);
}

bool type_name::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next<parse::instance>(i);
}

void type_name::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<type_name>())
	{
		tokens.register_syntax<type_name>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
	}
}

string type_name::to_string(string tab) const
{
	return name;
}

parse::syntax *type_name::clone() const
{
	return new type_name(*this);
}
}

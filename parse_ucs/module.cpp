/*
 * module.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "module.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs
{
module::module()
{
	debug_name = "module";
}

module::module(tokenizer &tokens, void *data)
{
	debug_name = "module";
	parse(tokens, data);
}

module::~module()
{

}

void module::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(false);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("module");

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		name = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.increment(false);
	tokens.expect<behavior>();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		behaviors.push_back(behavior(tokens, data));

		tokens.increment(false);
		tokens.expect<behavior>();
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool module::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("module", i);
}

void module::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<module>())
	{
		tokens.register_syntax<module>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(false);
		behavior::register_syntax(tokens);
	}
}

string module::to_string(string tab) const
{
	string result = "module";

	if (name != "")
		result += " " + name;

	result += "\n" + tab + "{\n";

	for (int i = 0; i < (int)behaviors.size(); i++)
		result += tab + "\t" + behaviors[i].to_string(tab + "\t") + "\n";

	result += tab + "}";

	return result;
}

parse::syntax *module::clone() const
{
	return new module(*this);
}
}

/*
 * behavior.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "behavior.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>

namespace parse_ucs
{
behavior::behavior()
{
	debug_name = "behavior";
	content = NULL;
}

behavior::behavior(const behavior &cpy) : parse::syntax(cpy)
{
	name = cpy.name;
	lang = cpy.lang;
	if (cpy.content != NULL)
		content = cpy.content->clone();
	else
		content = NULL;
}

behavior::behavior(tokenizer &tokens, void *data)
{
	debug_name = "behavior";
	content = NULL;
	parse(tokens, data);
}

behavior::~behavior()
{
	if (content != NULL)
		delete content;
	content = NULL;
}

void behavior::parse(tokenizer &tokens, void *data)
{
	map<string, language>::iterator iter = registry.end();

	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(false);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	for (map<string, language>::iterator i = registry.begin(); i != registry.end(); i++)
		tokens.expect(i->first);

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		lang = tokens.next();
		iter = registry.find(lang);
		if (iter == registry.end())
			tokens.error("Parser for '" + lang + "' not registered.", __FILE__, __LINE__);
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		name = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (iter != registry.end())
	{
		tokens.increment(true);
		tokens.expect(iter->second.type());

		if (tokens.decrement(__FILE__, __LINE__, data))
			content = iter->second.factory(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool behavior::is_next(tokenizer &tokens, int i, void *data)
{
	bool result = false;

	for (map<string, language>::iterator j = registry.begin(); j != registry.end(); j++)
		result = result || tokens.is_next(j->first, i);

	return result;
}

void behavior::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<behavior>())
	{
		tokens.register_syntax<behavior>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		member_name::register_syntax(tokens);
		for (map<string, language>::iterator i = registry.begin(); i != registry.end(); i++)
			i->second.register_syntax(tokens);
	}
}

string behavior::to_string(string tab) const
{
	string result = lang;

	if (name != "")
		result += " " + name;

	result += "\n" + tab + "\n" + tab + "\t";

	if (content != NULL)
		result += content->to_string(tab + "\t");

	result += "\n" + tab + "}";

	return result;
}

parse::syntax *behavior::clone() const
{
	return new behavior(*this);
}
}

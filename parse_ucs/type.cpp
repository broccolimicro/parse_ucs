/*
 * type.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "type.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>

namespace parse_ucs
{

vector<string> type::precedence;

type::type()
{
	debug_name = "type";
	level = 1;
	init();
}

type::type(tokenizer &tokens, int level, void *data)
{
	debug_name = "type";
	this->level = level;
	init();
	parse(tokens, data);
}

type::~type()
{

}

void type::init()
{
	if (precedence.size() == 0)
	{
		precedence.push_back(",");
		precedence.push_back("::");
	}
}

void type::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	bool first = true;
	do
	{
		if (first)
			first = false;
		else
			tokens.next();

		tokens.increment(false);
		tokens.expect(precedence[level]);

		tokens.increment(true);
		if (level < (int)precedence.size()-1)
			tokens.expect<type>();
		else
		{
			tokens.expect<type_name>();
			tokens.expect("{");
		}

		if (tokens.decrement(__FILE__, __LINE__, data))
		{
			if (tokens.found<type>())
				names.push_back(sub_type(type(tokens, level+1, data)));
			else if (tokens.found<type_name>())
				names.push_back(sub_type(type_name(tokens, data)));
			else if (tokens.found("{"))
			{
				tokens.next();

				tokens.increment(true);
				tokens.expect("}");

				tokens.increment(true);
				tokens.expect<type>();

				if (tokens.decrement(__FILE__, __LINE__, data))
					names.push_back(sub_type(type(tokens, 0, data)));

				if (tokens.decrement(__FILE__, __LINE__, data))
					tokens.next();
			}
		}
	} while (tokens.decrement(__FILE__, __LINE__, data));

	tokens.syntax_end(this);
}

bool type::is_next(tokenizer &tokens, int i, void *data)
{
	return type_name::is_next(tokens, i, data) || tokens.is_next("{", i);
}

void type::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<type>())
	{
		tokens.register_syntax<type>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::white_space>(false);
		type_name::register_syntax(tokens);
	}
}

string type::to_string(string tab) const
{
	return to_string(1, tab);
}

string type::to_string(int prev_level, string tab) const
{
	if (!valid || names.size() == 0)
		return "void";

	string result = "";
	if (prev_level > level)
		result += "{";

	for (int i = 0; i < (int)names.size(); i++)
	{
		if (i != 0)
			result += precedence[level];

		result += names[i].to_string(level, tab);
	}

	if (prev_level > level)
		result += "}";

	return result;
}

parse::syntax *type::clone() const
{
	return new type(*this);
}

sub_type::sub_type()
{

}

sub_type::sub_type(type sub)
{
	this->sub = sub;
}

sub_type::sub_type(type_name name)
{
	this->name = name;
}

sub_type::~sub_type()
{

}

string sub_type::to_string(int level, string tab) const
{
	if (sub.valid)
		return sub.to_string(level, tab);
	if (name.valid)
		return name.to_string(tab);
	else
		return "void";
}
}

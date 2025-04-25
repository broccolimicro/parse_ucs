#include "datatype.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs
{
datatype::datatype()
{
	debug_name = "datatype";
	isInterface = false;
}

datatype::datatype(tokenizer &tokens, void *data)
{
	debug_name = "datatype";
	isInterface = false;
	parse(tokens, data);
}

datatype::~datatype()
{

}

void datatype::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(false);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("type");
	tokens.expect("interface");

	if (tokens.decrement(__FILE__, __LINE__, data))
		isInterface = (tokens.next() == "interface");

	if (tokens.decrement(__FILE__, __LINE__, data))
		name = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.increment(false);
	tokens.expect<parse::new_line>();
	if (not isInterface) {
		tokens.expect<declaration>();
	}
	tokens.expect<prototype>();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found<parse::new_line>()) {
			tokens.next();
		} else if (tokens.found<prototype>()) {
			protocols.push_back(prototype(tokens, data));
		} else {
			members.push_back(declaration(tokens, data));
		}

		tokens.increment(false);
		if (not isInterface) {
			tokens.expect<declaration>();
		}
		tokens.expect<prototype>();
		tokens.expect<parse::new_line>();
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool datatype::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("type", i) or tokens.is_next("interface", i);
}

void datatype::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<datatype>())
	{
		tokens.register_syntax<datatype>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		declaration::register_syntax(tokens);
		prototype::register_syntax(tokens);
	}
}

string datatype::to_string(string tab) const
{
	string result = (isInterface ? "interface" : "type");

	if (name != "")
		result += " " + name;

	result += " {\n";

	for (int i = 0; i < (int)members.size(); i++) {
		result += tab + "\t" + members[i].to_string(tab + "\t") + "\n";
	}

	for (int i = 0; i < (int)protocols.size(); i++) {
		result += tab + "\t" + protocols[i].to_string(tab + "\t") + "\n";
	}

	result += tab + "}";

	return result;
}

parse::syntax *datatype::clone() const
{
	return new datatype(*this);
}
}

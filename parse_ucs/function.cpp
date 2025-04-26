#include "function.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs
{

map<string, language> function::registry;

language::language()
{
	factory = NULL;
	expect = NULL;
	register_syntax = NULL;
}

language::language(parse::syntax *(*factory)(tokenizer&, void*), void (*expect)(tokenizer&), void (*register_syntax)(tokenizer&))
{
	this->factory = factory;
	this->expect = expect;
	this->register_syntax = register_syntax;
}

language::~language()
{

}

function::function()
{
	debug_name = "function";
	body = nullptr;
}

function::function(const function &cpy) : parse::syntax(cpy) {
	lang = cpy.lang;
	name = cpy.name;
	recv = cpy.recv;
	args = cpy.args;
	ret = cpy.ret;
	if (cpy.body != nullptr) {
		body = cpy.body->clone();
	} else {
		body = nullptr;
	}
}

function::function(tokenizer &tokens, void *data)
{
	debug_name = "function";
	body = nullptr;
	parse(tokens, data);
}

function::~function()
{
	if (body != nullptr) {
		delete body;
	}
	body = nullptr;
}

void function::parse(tokenizer &tokens, void *data)
{
	auto iter = registry.end();

	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(false);
	tokens.expect<parse::new_line>();

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(false);
	tokens.expect(":");

	tokens.increment(false);
	tokens.expect<type_name>();

	tokens.increment(true);
	tokens.expect(")");

	tokens.increment(false);
	tokens.expect<declaration>();

	tokens.increment(true);
	tokens.expect("(");

	tokens.increment(false);
	tokens.expect("::");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	for (auto i = registry.begin(); i != registry.end(); i++) {
		tokens.expect(i->first);
	}

	// "func"
	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		lang = tokens.next();
		iter = registry.find(lang);
		if (iter == registry.end()) {
			tokens.error("Parser for '" + lang + "' not registered.", __FILE__, __LINE__);
		}
	}

	// name of function
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name = tokens.next();
	}

	// receiver
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		recv = name;

		tokens.next();

		tokens.increment(true);
		tokens.expect<parse::instance>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			name = tokens.next();
		}
	}

	// "("
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	// arguments
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		args.push_back(declaration(tokens, data));

		tokens.increment(false);
		tokens.expect(";");

		while (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();

			tokens.increment(true);
			tokens.expect<declaration>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				args.push_back(declaration(tokens, data));
			}

			tokens.increment(false);
			tokens.expect(";");
		}
	}

	// ")"
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	// return type
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		ret.parse(tokens, data);
	}

	// ":"
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(true);
		tokens.expect<parse::instance>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			impl = tokens.next();
		}
	}

	// "{"
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (iter != registry.end())
	{
		tokens.increment(false);
		iter->second.expect(tokens);

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			body = iter->second.factory(tokens, data);
		}
	}

	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect<parse::new_line>();
	}

	// "}"
	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool function::is_next(tokenizer &tokens, int i, void *data)
{
	bool result = false;

	for (auto j = registry.begin(); j != registry.end(); j++) {
		result = result or tokens.is_next(j->first, i);
	}

	return result;
}

void function::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<function>())
	{
		tokens.register_syntax<function>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		type_name::register_syntax(tokens);
		for (auto i = registry.begin(); i != registry.end(); i++) {
			i->second.register_syntax(tokens);
		}
	}
}

string function::to_string(string tab) const {
	string result = lang;

	if (name != "") {
		result += " ";
		if (recv != "") {
			result += recv + "::";
		}
		result += name;
	}

	result += "(";
	for (auto i = args.begin(); i != args.end(); i++) {
		if (i != args.begin()) {
			result += "; ";
		}
		result += i->to_string(tab);
	}

	result += ")";

	if (ret.valid) {
		result += " " + ret.to_string(tab);
	}

	if (impl != "") {
		result += " : " + impl;
	}

	result += " {\n";

	if (body != nullptr) {
		result += body->to_string(tab + "\t");
	}

	result += "\n" + tab + "}";

	return result;
}

parse::syntax *function::clone() const
{
	return new function(*this);
}
}

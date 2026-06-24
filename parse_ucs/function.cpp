#include "function.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

map<string, language> function::registry;

language::language() {
	factory = NULL;
	expect = NULL;
	register_syntax = NULL;
}

language::language(parse::syntax *(*factory)(tokenizer&, void*), void (*expect)(tokenizer&), void (*register_syntax)(tokenizer&)) {
	this->factory = factory;
	this->expect = expect;
	this->register_syntax = register_syntax;
}

language::~language() {
}

function::function() {
	debug_name = "wv_function";
	body = nullptr;
}

function::function(const function &cpy) : parse::syntax(cpy) {
	lang = cpy.lang;
	decl = cpy.decl;
	if (cpy.body != nullptr) {
		body = cpy.body->clone();
	} else {
		body = nullptr;
	}
}

function::function(tokenizer &tokens, void *data) {
	debug_name = "wv_function";
	body = nullptr;
	parse(tokens, data);
}

function::~function() {
	if (body != nullptr) {
		delete body;
	}
	body = nullptr;
}

void function::parse(tokenizer &tokens, void *data) {
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

	tokens.increment(true);
	tokens.expect<function_decl>();

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

	// function declaration
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		decl.parse(tokens, data);
	}

	// ":"
	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect(",");

		tokens.increment(true);
		tokens.expect<signature>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			impl.push_back(signature(tokens, data));
		}
	}

	// "{"
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (iter != registry.end()) {
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
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool function::is_next(tokenizer &tokens, int i, void *data) {
	bool result = false;

	for (auto j = registry.begin(); j != registry.end(); j++) {
		result = result or tokens.is_next(j->first, i);
	}

	return result;
}

void function::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<function>()) {
		tokens.register_syntax<function>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		signature::register_syntax(tokens);
		function_decl::register_syntax(tokens);
		for (auto i = registry.begin(); i != registry.end(); i++) {
			i->second.register_syntax(tokens);
		}
	}
}

string function::to_string(string tab) const {
	string result = lang;
	if (decl.valid) {
		if (not decl.name.empty()) {
			result += " ";
		}
		result += decl.to_string(tab);
	}

	if (not impl.empty()) {
		for (int i = 0; i < (int)impl.size(); i++) {
			if (i == 0) {
				result += " : " + impl[i].to_string("");
			} else {
				result += ", " + impl[i].to_string("");
			}
		}
	}

	result += " {\n";

	if (body != nullptr) {
		result += body->to_string(tab + "\t");
	}

	result += "\n" + tab + "}";

	return result;
}

parse::syntax *function::clone() const {
	return new function(*this);
}

}

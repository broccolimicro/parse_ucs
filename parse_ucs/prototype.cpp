#include "prototype.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

prototype::prototype() {
	debug_name = "prototype";
}

prototype::prototype(tokenizer &tokens, void *data) {
	debug_name = "prototype";
	parse(tokens, data);
}

prototype::~prototype() {
}

void prototype::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);
	
	tokens.increment(false);
	tokens.expect<type_name>();

	tokens.increment(true);
	tokens.expect(")");

	tokens.increment(false);
	tokens.expect<declaration>();

	tokens.increment(true);
	tokens.expect("(");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	// function name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name = tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

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

	tokens.syntax_end(this);
}

bool prototype::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next<parse::instance>(i) and tokens.is_next("(", i+1) and not tokens.is_next("func", i) and not tokens.is_next("struct", i);
}

void prototype::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<prototype>())
	{
		tokens.register_syntax<prototype>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		declaration::register_syntax(tokens);
	}
}

string prototype::to_string(string tab) const {
	string result = name + "(";
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

	return result;
}

parse::syntax *prototype::clone() const
{
	return new prototype(*this);
}

}


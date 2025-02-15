#include "declaration.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

declaration::declaration() {
	debug_name = "declaration";
}

declaration::declaration(tokenizer &tokens, void *data) {
	debug_name = "declaration";
	parse(tokens, data);
}

declaration::~declaration() {
}

void declaration::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect("=");

	tokens.increment(false);
	tokens.expect(",");
	
	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect<parse::instance>();

	// type name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		type = tokens.next();
	}

	// variable name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name.push_back(tokens.next());
	}

	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	
		tokens.increment(false);
		tokens.expect(",");

		tokens.increment(true);
		tokens.expect<parse::instance>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			name.push_back(tokens.next());
		}
	}

	// reset behavior
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		if (reset.size()+1 < name.size()) {
			tokens.increment(true);
			tokens.expect(",");
		}

		tokens.increment(true);
		tokens.expect<parse_expression::expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			reset.push_back(parse_expression::expression(tokens, 0, data));
		}

		while (reset.size() < name.size() and tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();

			if (reset.size()+1 < name.size()) {
				tokens.increment(true);
				tokens.expect(",");
			}

			tokens.increment(true);
			tokens.expect<parse_expression::expression>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				reset.push_back(parse_expression::expression(tokens, 0, data));
			}
		}
	}

	tokens.syntax_end(this);
}

bool declaration::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next<parse::instance>() and not tokens.is_next("func") and not tokens.is_next("struct");
}

void declaration::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<declaration>())
	{
		tokens.register_syntax<declaration>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		parse_expression::expression::register_syntax(tokens);
	}
}

string declaration::to_string(string tab) const {
	string result = type + " ";
	for (auto i = name.begin(); i != name.end(); i++) {
		if (i != name.begin()) {
			result += ", ";
		}
		result += *i;
	}
	if (not reset.empty()) {
		result += " = ";
		for (auto i = reset.begin(); i != reset.end(); i++) {
			if (i != reset.begin()) {
				result += ", ";
			}
			result += i->to_string(tab);
		}
	}

	return result;
}

parse::syntax *declaration::clone() const
{
	return new declaration(*this);
}

}


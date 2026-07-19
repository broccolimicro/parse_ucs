#include "type_signature.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

type_signature::type_signature() {
	debug_name = "wv_type_signature";
}

type_signature::type_signature(tokenizer &tokens, std::any data) {
	debug_name = "wv_type_signature";
	parse(tokens, data);
}

type_signature::~type_signature() {
}

void type_signature::parse(tokenizer &tokens, std::any data) {
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect("[");

	tokens.increment(true);
	tokens.expect<type_name>();

	if (tokens.decrement(__FILE__, __LINE__)) {
		type.parse(tokens, data);
	}

	while (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect("[");

		tokens.increment(true);
		tokens.expect("]");

		tokens.increment(true);
		tokens.expect<expression>();

		if (tokens.decrement(__FILE__, __LINE__)) {
			size.push_back(expression(tokens, data));
		}

		if (tokens.decrement(__FILE__, __LINE__)) {
			tokens.next();
		}
	}

	tokens.syntax_end(this);
}

bool type_signature::is_next(tokenizer &tokens, int i, std::any data) {
	return type_name::is_next(tokens, i, data)
		and not tokens.is_next("func", i)
		and not tokens.is_next("struct", i);
}

void type_signature::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<type_signature>()) {
		tokens.register_syntax<type_signature>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		type_name::register_syntax(tokens);
		expression::register_syntax(tokens);
	}
}

string type_signature::to_string(string tab) const {
	string result = type.to_string(tab);
	
	for (auto i = size.begin(); i != size.end(); i++) {
		result += "[" + i->to_string(tab) + "]";
	}

	return result;
}

parse::syntax *type_signature::clone() const {
	return new type_signature(*this);
}

}


#include "signature.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

signature::signature() {
	debug_name = "wv_signature";
	unqualified = true;
}

signature::signature(tokenizer &tokens, void *data) {
	debug_name = "wv_signature";
	unqualified = true;
	parse(tokens, data);
}

signature::~signature() {
}

void signature::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect("(");

	tokens.increment(false);
	tokens.expect("::");

	tokens.increment(true);
	tokens.expect<type_name>();

	// function name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		recv.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(true);
		tokens.expect<parse::instance>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			name = tokens.next();
		}
	} else {
		name = recv.name;
		recv.name = "";
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		unqualified = false;
		tokens.next();

		tokens.increment(true);
		tokens.expect(")");

		tokens.increment(false);
		tokens.expect<type_signature>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			args.push_back(type_signature(tokens, data));

			tokens.increment(false);
			tokens.expect(",");

			while (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();

				tokens.increment(true);
				tokens.expect<type_signature>();

				if (tokens.decrement(__FILE__, __LINE__, data)) {
					args.push_back(type_signature(tokens, data));
				}

				tokens.increment(false);
				tokens.expect(",");
			}
		}

		// ")"
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
	}

	tokens.syntax_end(this);
}

bool signature::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next<parse::instance>(i)
		and not tokens.is_next("func", i)
		and not tokens.is_next("struct", i);
}

void signature::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<signature>()) {
		tokens.register_syntax<signature>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		type_name::register_syntax(tokens);
		type_signature::register_syntax(tokens);
	}
}

string signature::to_string(string tab) const {
	string result;
	if (recv.name != "") {
		result += recv.to_string(tab) + "::";
	} else {
		result += recv.mod + ".";
	}

	result += name + "(";
	for (auto i = args.begin(); i != args.end(); i++) {
		if (i != args.begin()) {
			result += "; ";
		}
		result += i->to_string(tab);
	}
	result += ")";

	return result;
}

parse::syntax *signature::clone() const {
	return new signature(*this);
}

}


#include "function_decl.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

function_decl::function_decl() {
	debug_name = "wv_function_decl";
}

function_decl::function_decl(tokenizer &tokens, std::any data) {
	debug_name = "wv_function_decl";
	parse(tokens, data);
}

function_decl::~function_decl() {
}

void function_decl::parse(tokenizer &tokens, std::any data) {
	tokens.syntax_start(this);

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

	// name of function
	if (tokens.decrement(__FILE__, __LINE__)) {
		name = tokens.next();
	}

	// receiver
	if (tokens.decrement(__FILE__, __LINE__)) {
		recv = name;

		tokens.next();

		tokens.increment(true);
		tokens.expect<parse::instance>();

		if (tokens.decrement(__FILE__, __LINE__)) {
			name = tokens.next();
		}
	}

	// "("
	if (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();
	}

	// arguments
	if (tokens.decrement(__FILE__, __LINE__)) {
		args.push_back(declaration(tokens, data));

		tokens.increment(false);
		tokens.expect(";");

		while (tokens.decrement(__FILE__, __LINE__)) {
			tokens.next();

			tokens.increment(true);
			tokens.expect<declaration>();

			if (tokens.decrement(__FILE__, __LINE__)) {
				args.push_back(declaration(tokens, data));
			}

			tokens.increment(false);
			tokens.expect(";");
		}
	}

	// ")"
	if (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();
	}

	// return type
	if (tokens.decrement(__FILE__, __LINE__)) {
		ret.parse(tokens, data);
	}

	tokens.syntax_end(this);
}

bool function_decl::is_next(tokenizer &tokens, int i, std::any data) {
	return tokens.is_next<parse::instance>(i);
}

void function_decl::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<function_decl>()) {
		tokens.register_syntax<function_decl>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
	}
}

string function_decl::to_string(string tab) const {
	string result;
	if (name != "") {
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

	return result;
}

parse::syntax *function_decl::clone() const {
	return new function_decl(*this);
}

}

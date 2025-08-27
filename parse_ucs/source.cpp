#include "source.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

source::source() {
	debug_name = "source";
}

source::source(tokenizer &tokens, void *data) {
	debug_name = "source";
	parse(tokens, data);
}

source::~source() {
}

void source::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect<include>();
	tokens.expect<parse::new_line>();
	while (tokens.decrement(__FILE__, __LINE__)) {
		if (tokens.found<include>()) {
			incl.push_back(include(tokens, data));
		} else {
			tokens.next();
		}

		tokens.increment(false);
		tokens.expect<include>();
		tokens.expect<parse::new_line>();
	}

	tokens.increment(false);
	tokens.expect<function>();
	tokens.expect<datatype>();
	tokens.expect<parse::new_line>();
	while (tokens.decrement(__FILE__, __LINE__)) {
		if (tokens.found<function>()) {
			funcs.push_back(function(tokens, data));
		} else if (tokens.found<datatype>()) {
			types.push_back(datatype(tokens, data));
		} else {
			tokens.next();
		}

		tokens.increment(false);
		tokens.expect<function>();
		tokens.expect<datatype>();
		tokens.expect<parse::new_line>();
	}

	tokens.syntax_end(this);
}

bool source::is_next(tokenizer &tokens, int i, void *data) {
	while (tokens.is_next<parse::new_line>(i)) {
		i++;
	}

	return include::is_next(tokens, i, data) or datatype::is_next(tokens, i, data) or function::is_next(tokens, i, data);
}

void source::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<source>()) {
		tokens.register_syntax<source>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		function::register_syntax(tokens);
		datatype::register_syntax(tokens);
		include::register_syntax(tokens);
	}
}

string source::to_string(string tab) const {
	string result = "";

	for (auto i = incl.begin(); i != incl.end(); i++) {
		result += i->to_string(tab);
	}

	for (auto i = types.begin(); i != types.end(); i++) {
		result += i->to_string(tab);
		result += "\n\n";
	}

	for (auto i = funcs.begin(); i != funcs.end(); i++) {
		result += i->to_string(tab);
		result += "\n\n";
	}

	return result;
}

parse::syntax *source::clone() const {
	return new source(*this);
}

}

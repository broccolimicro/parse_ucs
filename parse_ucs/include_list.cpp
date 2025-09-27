#include "include_list.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

include_list::include_list() {
	debug_name = "wv_include_list";
}

include_list::include_list(tokenizer &tokens, void *data) {
	debug_name = "wv_include_list";
	parse(tokens, data);
}

include_list::~include_list() {
}

void include_list::parse(tokenizer &tokens, void *data) {
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

	tokens.syntax_end(this);
}

bool include_list::is_next(tokenizer &tokens, int i, void *data) {
	while (tokens.is_next<parse::new_line>(i)) {
		i++;
	}

	return include::is_next(tokens, i, data);
}

void include_list::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<include_list>()) {
		tokens.register_syntax<include_list>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		include::register_syntax(tokens);
	}
}

string include_list::to_string(string tab) const {
	string result = "";

	for (auto i = incl.begin(); i != incl.end(); i++) {
		result += i->to_string(tab);
	}

	return result;
}

parse::syntax *include_list::clone() const {
	return new include_list(*this);
}

}

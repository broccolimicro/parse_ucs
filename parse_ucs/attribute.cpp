#include "attribute.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/text.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

attribute::attribute() {
	debug_name = "wv_attribute";
}

attribute::attribute(tokenizer &tokens, void *data) {
	debug_name = "wv_attribute";
	parse(tokens, data);
}

attribute::~attribute() {
}

void attribute::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);
	
	tokens.increment(true);
	tokens.expect<parse::new_line>();

	tokens.increment(true);
	tokens.expect<parse::text>();

	tokens.increment(true);
	tokens.expect<parse::instance>();
	
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name = tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		value = tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool attribute::is_next(tokenizer &tokens, int i, void *data) {
	if (not tokens.is_next<parse::instance>(i)
		or not tokens.is_next<parse::text>(i+1)) {
		return false;
	}

	string attr = tokens.peek(i);
	return attr == "module"
		or attr == "tech";
}

void attribute::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<attribute>()) {
		tokens.register_syntax<attribute>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::text>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
	}
}

string attribute::to_string(string tab) const {
	return name + " " + value + "\n";
}

parse::syntax *attribute::clone() const {
	return new attribute(*this);
}

}


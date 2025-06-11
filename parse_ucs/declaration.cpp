#include "declaration.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

variable::variable() {
}

variable::variable(string name) {
	this->name = name;
}

variable::~variable() {
}

string variable::to_string(string tab) const {
	string result = name;
	for (int i = 0; i < (int)size.size(); i++) {
		result += "[" + size[i].to_string(tab) + "]";
	}
	return result;
}

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

	tokens.increment(false);
	tokens.expect("[");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect<type_name>();

	// type name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		type.parse(tokens, data);
	}

	// variable name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name.push_back(variable(tokens.next()));
	}

	// [
	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect("[");

		tokens.increment(true);
		tokens.expect("]");

		tokens.increment(true);
		tokens.expect<parse_expression::expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			name.back().size.push_back(parse_expression::expression(tokens, 0, data));
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
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
	if (tokens.is_next("func", i)
		or tokens.is_next("struct", i)
		or tokens.is_next("interface", i)
		or tokens.is_next("context", i)
		or tokens.is_next("await", i)
		or tokens.is_next("while", i)
		or tokens.is_next("region", i)
		or tokens.is_next("assume", i)) {
		return false;
	}

	return type_name::is_next(tokens, i, data);
}

void declaration::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<declaration>()) {
		tokens.register_syntax<declaration>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		type_name::register_syntax(tokens);
		parse_expression::expression::register_syntax(tokens);
	}
}

string declaration::to_string(string tab) const {
	string result = type.to_string(tab) + " ";
	for (auto i = name.begin(); i != name.end(); i++) {
		if (i != name.begin()) {
			result += ", ";
		}
		result += i->to_string(tab);
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

parse::syntax *declaration::clone() const {
	return new declaration(*this);
}



inline_declaration::inline_declaration() {
	debug_name = "inline_declaration";
}

inline_declaration::inline_declaration(tokenizer &tokens, void *data) {
	debug_name = "inline_declaration";
	parse(tokens, data);
}

inline_declaration::~inline_declaration() {
}

void inline_declaration::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect("=");

	tokens.increment(false);
	tokens.expect(",");

	tokens.increment(false);
	tokens.expect("[");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect<type_name>();

	tokens.increment(true);
	tokens.expect("var");

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	// type name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		type.parse(tokens, data);
	}

	// variable name
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name.push_back(variable(tokens.next()));
	}

	// [
	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect("[");

		tokens.increment(true);
		tokens.expect("]");

		tokens.increment(true);
		tokens.expect<parse_expression::expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			name.back().size.push_back(parse_expression::expression(tokens, 0, data));
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
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

bool inline_declaration::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("var", i);
}

void inline_declaration::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<inline_declaration>()) {
		tokens.register_syntax<inline_declaration>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		type_name::register_syntax(tokens);
		parse_expression::expression::register_syntax(tokens);
	}
}

string inline_declaration::to_string(string tab) const {
	string result = "var " + type.to_string(tab) + " ";
	for (auto i = name.begin(); i != name.end(); i++) {
		if (i != name.begin()) {
			result += ", ";
		}
		result += i->to_string(tab);
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

parse::syntax *inline_declaration::clone() const {
	return new inline_declaration(*this);
}

}


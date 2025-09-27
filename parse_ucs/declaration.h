#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>
#include "type_name.h"

namespace parse_ucs {

template <typename expression>
struct variable_t {
	using variable=parse_ucs::variable_t<expression>;

	string name;
	vector<expression> size;

	variable_t() {}
	variable_t(string name) : name(name) {}
	~variable_t() {}

	string to_string(string tab="") const {
		string result = name;
		for (int i = 0; i < (int)size.size(); i++) {
			result += "[" + size[i].to_string(tab) + "]";
		}
		return result;
	}
};

template <typename expression>
struct declaration_t : parse::syntax {
	using variable=parse_ucs::variable_t<expression>;
	using declaration=parse_ucs::declaration_t<expression>;

	type_name type;
	vector<variable> name;
	vector<expression> reset;

	declaration_t() {
		debug_name = "wv_declaration";
	}

	declaration_t(tokenizer &tokens, void *data=nullptr) {
		debug_name = "wv_declaration_";
		parse(tokens, data);
	}

	~declaration_t() {
	}

	void parse(tokenizer &tokens, void *data=nullptr) {
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
			tokens.expect<expression>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				name.back().size.push_back(expression(tokens, 0, data));
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
			tokens.expect<expression>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				reset.push_back(expression(tokens, 0, data));
			}

			while (reset.size() < name.size() and tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();

				if (reset.size()+1 < name.size()) {
					tokens.increment(true);
					tokens.expect(",");
				}

				tokens.increment(true);
				tokens.expect<expression>();

				if (tokens.decrement(__FILE__, __LINE__, data)) {
					reset.push_back(expression(tokens, 0, data));
				}
			}
		}

		tokens.syntax_end(this);
	}

	static bool is_next(tokenizer &tokens, int i=1, void *data=nullptr) {
		return type_name::is_next(tokens, i, data);
	}

	static void register_syntax(tokenizer &tokens) {
		if (!tokens.syntax_registered<declaration>()) {
			tokens.register_syntax<declaration>();
			tokens.register_token<parse::symbol>();
			tokens.register_token<parse::instance>();
			tokens.register_token<parse::white_space>(false);
			tokens.register_token<parse::new_line>(true);
			type_name::register_syntax(tokens);
			expression::register_syntax(tokens);
		}
	}

	string to_string(string tab="") const {
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

	parse::syntax *clone() const {
		return new declaration(*this);
	}
};

}

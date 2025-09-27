#include "require.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/text.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

require::require() {
	debug_name = "wv_require";
}

require::require(tokenizer &tokens, void *data) {
	debug_name = "wv_require";
	parse(tokens, data);
}

require::~require() {
}

void require::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);
	
	tokens.increment(true);
	tokens.expect<parse::new_line>();

	tokens.increment(false);
	tokens.expect("(");

	tokens.increment(true);
	tokens.expect("require");
	
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
		
		tokens.increment(true);
		tokens.expect(")");

		tokens.increment(false);
		tokens.expect<parse::text>();

		tokens.increment(true);
		tokens.expect<parse::new_line>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}

		while (tokens.decrement(__FILE__, __LINE__, data)) {
			path.push_back({tokens.next(), ""});

			tokens.increment(true);
			tokens.expect<parse::text>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				path.back().second = tokens.next();
			}

			tokens.increment(false);
			tokens.expect<parse::text>();

			tokens.increment(true);
			tokens.expect<parse::new_line>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
	} else {
		tokens.increment(true);
		tokens.expect<parse::text>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			path.push_back({tokens.next(), ""});

			tokens.increment(true);
			tokens.expect<parse::text>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				path.back().second = tokens.next();
			}
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool require::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("require", i);
}

void require::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<require>())
	{
		tokens.register_syntax<require>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::text>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
	}
}

string require::to_string(string tab) const {
	string result = "require (\n";
	for (auto i = path.begin(); i != path.end(); i++) {
		result += tab + "\t" + i->first;
		if (i->first != "") {
			result += " " + i->second;
		}
		result += "\n";
	}
	result += tab + ")\n";
	return result;
}

parse::syntax *require::clone() const {
	return new require(*this);
}

}


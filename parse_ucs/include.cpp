#include "include.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/text.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

include::include() {
	debug_name = "wv_include";
}

include::include(tokenizer &tokens, void *data) {
	debug_name = "wv_include";
	parse(tokens, data);
}

include::~include() {
}

void include::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);
	
	tokens.increment(true);
	tokens.expect<parse::new_line>();

	tokens.increment(false);
	tokens.expect("(");

	tokens.increment(true);
	tokens.expect("import");
	
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
		
		tokens.increment(true);
		tokens.expect(")");

		tokens.increment(false);
		tokens.expect("as");

		tokens.increment(false);
		tokens.expect<parse::text>();

		tokens.increment(true);
		tokens.expect<parse::new_line>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}

		while (tokens.decrement(__FILE__, __LINE__, data)) {
			path.push_back({"", tokens.next()});

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();

				tokens.increment(true);
				tokens.expect<parse::instance>();
	
				if (tokens.decrement(__FILE__, __LINE__, data)) {
					path.back().first = tokens.next();
				}
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
		tokens.increment(false);
		tokens.expect("as");

		tokens.increment(true);
		tokens.expect<parse::text>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			path.push_back({"", tokens.next()});

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();

				tokens.increment(true);
				tokens.expect<parse::instance>();
	
				if (tokens.decrement(__FILE__, __LINE__, data)) {
					path.back().first = tokens.next();
				}
			}
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool include::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("import", i);
}

void include::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<include>())
	{
		tokens.register_syntax<include>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::text>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
	}
}

string include::to_string(string tab) const {
	string result = "import (\n";
	for (auto i = path.begin(); i != path.end(); i++) {
		result += tab + "\t" + i->second;
		if (i->first != "") {
			result += " as " + i->first;
		}
		result += "\n";
	}
	result += tab + ")\n";
	return result;
}

parse::syntax *include::clone() const {
	return new include(*this);
}

}


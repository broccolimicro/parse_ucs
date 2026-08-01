#include "function.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs {

function::function() {
	debug_name = "wv_function";
}

function::function(tokenizer &tokens, std::any data) {
	debug_name = "wv_function";
	body = nullptr;
	parse(tokens, data);
}

function::~function() {
}

void function::parse(tokenizer &tokens, std::any data) {
	if (not data.has_value()) {
		tokens.internal("parsing registry not loaded", __FILE__, __LINE__);
		return;
	}

	const parse::registry *registry = std::any_cast<const parse::registry*>(data);

	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(false);
	tokens.expect<parse::new_line>();

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(false);
	tokens.expect(":");

	tokens.increment(true);
	tokens.expect<function_decl>();

	tokens.increment(true);
	for (std::string lang : registry->getParserIndex()) {
		tokens.expect(lang);
	}

	// "func"
	const parse::factory *ref = nullptr;
	if (tokens.decrement(__FILE__, __LINE__)) {
		lang = tokens.next();
		ref = registry->getParser(lang);
		if (ref == nullptr) {
			tokens.error("Parser for '" + lang + "' not registered.", __FILE__, __LINE__);
		}
	}

	// function declaration
	if (tokens.decrement(__FILE__, __LINE__)) {
		decl.parse(tokens);
	}

	// ":"
	while (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect(",");

		tokens.increment(true);
		tokens.expect<signature>();

		if (tokens.decrement(__FILE__, __LINE__)) {
			impl.push_back(signature(tokens));
		}
	}

	// "{"
	if (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();
	}

	if (ref != nullptr) {
		tokens.increment(false);
		ref->expect(tokens);

		if (tokens.decrement(__FILE__, __LINE__)) {
			body = std::shared_ptr<parse::syntax>(ref->produce(tokens));
		}
	}

	while (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect<parse::new_line>();
	}

	// "}"
	if (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool function::is_next(tokenizer &tokens, int i, std::any data) {
	if (not data.has_value()) {
		tokens.internal("parsing registry not loaded", __FILE__, __LINE__);
		return false;
	}

	const parse::registry *registry = std::any_cast<const parse::registry*>(data);

	for (std::string lang : registry->getParserIndex()) {
		if (tokens.is_next(lang, i)) {
			return true;
		}
	}
	return false;
}

void function::register_syntax(tokenizer &tokens, const parse::registry *registry) {
	if (!tokens.syntax_registered<function>()) {
		tokens.register_syntax<function>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		signature::register_syntax(tokens);
		function_decl::register_syntax(tokens);
		if (registry != nullptr) {
			for (std::string name : registry->getParserIndex()) {
				const parse::factory *lang = registry->getParser(name);
				if (lang != nullptr) {
					lang->register_syntax(tokens);
				}
			}
		} else {
			tokens.internal("parsing registry not loaded", __FILE__, __LINE__);
		}
	}
}

string function::to_string(string tab) const {
	string result = lang;
	if (decl.valid) {
		if (not decl.name.empty()) {
			result += " ";
		}
		result += decl.to_string(tab);
	}

	if (not impl.empty()) {
		for (int i = 0; i < (int)impl.size(); i++) {
			if (i == 0) {
				result += " : " + impl[i].to_string("");
			} else {
				result += ", " + impl[i].to_string("");
			}
		}
	}

	result += " {\n";

	if (body) {
		result += body->to_string(tab + "\t");
	}

	result += "\n" + tab + "}";

	return result;
}

parse::syntax *function::clone() const {
	return new function(*this);
}

}

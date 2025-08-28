#include "modfile.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs
{

modfile::modfile()
{
	debug_name = "modfile";
}

modfile::modfile(tokenizer &tokens, void *data)
{
	debug_name = "modfile";
	parse(tokens, data);
}

modfile::~modfile()
{

}

void modfile::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect<require>();
	tokens.expect<attribute>();
	tokens.expect<parse::new_line>();
	while (tokens.decrement(__FILE__, __LINE__)) {
		if (tokens.found<require>()) {
			deps.push_back(require(tokens, data));
		} else if (tokens.found<attribute>()) {
			attrs.push_back(attribute(tokens, data));
		} else {
			tokens.next();
		}

		tokens.increment(false);
		tokens.expect<require>();
		tokens.expect<attribute>();
		tokens.expect<parse::new_line>();
	}

	tokens.syntax_end(this);
}

bool modfile::is_next(tokenizer &tokens, int i, void *data) {
	while (tokens.is_next<parse::new_line>(i)) {
		i++;
	}

	return require::is_next(tokens, i, data) or attribute::is_next(tokens, i, data);
}

void modfile::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<modfile>())
	{
		tokens.register_syntax<modfile>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		require::register_syntax(tokens);
		attribute::register_syntax(tokens);
	}
}

string modfile::to_string(string tab) const
{
	string result = "";

	for (auto i = attrs.begin(); i != attrs.end(); i++) {
		result += i->to_string(tab);
	}

	for (auto i = deps.begin(); i != deps.end(); i++) {
		result += i->to_string(tab);
	}

	return result;
}

parse::syntax *modfile::clone() const {
	return new modfile(*this);
}

}

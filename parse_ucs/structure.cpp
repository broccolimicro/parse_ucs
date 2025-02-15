#include "structure.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_ucs
{
structure::structure()
{
	debug_name = "structure";
}

structure::structure(tokenizer &tokens, void *data)
{
	debug_name = "structure";
	parse(tokens, data);
}

structure::~structure()
{

}

void structure::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(false);
	tokens.expect<parse::new_line>();
	tokens.expect<declaration>();

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(false);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("struct");

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		name = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found<parse::new_line>()) {
			tokens.next();
		} else {
			members.push_back(declaration(tokens, data));
		}

		tokens.increment(false);
		tokens.expect<declaration>();
		tokens.expect<parse::new_line>();
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool structure::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("struct", i);
}

void structure::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<structure>())
	{
		tokens.register_syntax<structure>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		declaration::register_syntax(tokens);
	}
}

string structure::to_string(string tab) const
{
	string result = "struct";

	if (name != "")
		result += " " + name;

	result += "\n" + tab + "{\n";

	for (int i = 0; i < (int)members.size(); i++)
		result += tab + "\t" + members[i].to_string(tab + "\t") + "\n";

	result += tab + "}";

	return result;
}

parse::syntax *structure::clone() const
{
	return new structure(*this);
}
}

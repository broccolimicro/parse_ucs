#pragma once

#include <parse/factory.h>

#include "declaration.h"
#include "type_name.h"
#include "expression.h"
#include "signature.h"
#include "function_decl.h"

#include <memory>

namespace parse_ucs {

// This represent **desired behaviors**
// 1. Behavioral
// 2. Functional
struct function : parse::syntax {
	using declaration=parse_ucs::declaration_t<expression>;

	function();
	function(tokenizer &tokens, std::any data=std::any());
	~function();

	string lang;
	function_decl decl;

	vector<signature> impl;

	std::shared_ptr<parse::syntax> body;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens, const parse::registry *registry);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

/*
 * language.h
 *
 *  Created on: Aug 17, 2015
 *      Author: nbingham
 */

#include <common/standard.h>
#include <parse/syntax.h>
#include <parse/tokenizer.h>

#ifndef language_h
#define language_h

struct language
{
	language();
	language(parse::syntax *(*factory)(tokenizer&, void*), string (*type)(), void (*register_syntax)(tokenizer&));
	~language();

	parse::syntax *(*factory)(tokenizer&, void*);
	string (*type)();
	void (*register_syntax)(tokenizer&);
};

#endif

/*
 * language.cpp
 *
 *  Created on: Aug 17, 2015
 *      Author: nbingham
 */

#include "language.h"

language::language()
{
	factory = NULL;
	type = NULL;
	register_syntax = NULL;
}

language::language(parse::syntax *(*factory)(tokenizer&, void*), string (*type)(), void (*register_syntax)(tokenizer&))
{
	this->factory = factory;
	this->type = type;
	this->register_syntax = register_syntax;
}

language::~language()
{

}

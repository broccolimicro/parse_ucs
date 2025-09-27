#pragma once

#include <parse_expression/expression.h>
#include <parse_expression/assignment.h>

namespace parse_ucs {

static const size_t expr_group = 0;

using expression=parse_expression::expression_t<expr_group>;
using assignment=parse_expression::assignment_t<expr_group>;

void setup_expressions();

}

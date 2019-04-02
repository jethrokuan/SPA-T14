#include "query_builder/pql/matcher.h"
#include "query_builder/core/exceptions.h"
#include "simple_parser/exceptions.h"
#include "simple_parser/interface.h"

using QE::Matcher;

Matcher::Matcher(bool isPartial_, std::string expr_)
    : isPartial(isPartial_), expr(expr_) {
  try {
    Simple::SimpleInterface::parseExpression(expr_);
  } catch (Simple::SimpleParseException e) {
    throw PQLParseException("'" + expr_ + "' does not fit expression spec.");
  }
};

bool Matcher::operator==(const Matcher& other) const {
  return isPartial == other.isPartial && expr.compare(other.expr) == 0;
}

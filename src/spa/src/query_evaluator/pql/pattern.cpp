#include "query_evaluator/pql/pattern.h"
using namespace QE;

// Does nothing else for now - but we could move some logic here (like semantics
// checking)
std::optional<Pattern> Pattern::construct(Synonym& syn, EntRef& a1,
                                          ExpressionSpec& a2) {
  return Pattern(syn, a1, a2);
}

// Does nothing else for now - but we could move some logic here (like semantics
// checking)
std::optional<Pattern*> Pattern::construct_heap(Synonym& syn, EntRef& a1,
                                                ExpressionSpec& a2) {
  return new Pattern(syn, a1, a2);
}
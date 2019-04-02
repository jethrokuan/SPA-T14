#include "query_builder/pql/with_cond.h"
#include "query_builder/core/exceptions.h"

using namespace QE;

WithCond::WithCond(AttrRef ref1_, AttrRef ref2_) : ref1(ref1_), ref2(ref2_) {
  if (ref1_.attrType != ref2_.attrType) {
    throw PQLParseException(
        "Cannot compare attribute refs of differing types.");
  }
}

bool WithCond::operator==(const WithCond& other) const {
  return ref1 == other.ref1 && ref2 == other.ref2;
}

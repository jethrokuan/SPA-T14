#pragma once
#include "query_builder/pql/ref.h"
#include "query_builder/pql/relation.h"

namespace QE {
class RelCond {
 public:
  Relation relation;
  Ref arg1;
  Ref arg2;
  RelCond(Relation relation_, Ref arg1_, Ref arg2_)
      : relation(relation_), arg1(arg1_), arg2(arg2_){};
  bool operator==(const RelCond& other) const {
    return relation == other.relation && arg1 == other.arg1 &&
           arg2 == other.arg2;
  }
  friend std::ostream& operator<<(std::ostream& os, RelCond const& relcond) {
    os << getStringFromRelation(relcond.relation) << "(..)";
    // os << relcond.arg1 << "," << relcond.arg2 << ")";
    return os;
  }
};
}  // namespace QE

#include "query_builder/pql/attrref.h"

namespace QE {
class WithCond {
 public:
  AttrRef ref1;
  AttrRef ref2;
  WithCond(AttrRef ref1_, AttrRef ref2_);

  bool operator==(const WithCond& other) const;
  friend std::ostream& operator<<(std::ostream& os, WithCond const&) {
    os << "with ... = ...";
    // os << relcond.arg1 << "," << relcond.arg2 << ")";
    return os;
  }
};
}  // namespace QE

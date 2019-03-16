#include <optional>
#include "query_builder/pql/attrref.h"

namespace QE {
class WithCond {
 public:
  AttrRef ref1;
  AttrRef ref2;
  WithCond(AttrRef ref1_, AttrRef ref2_) : ref1(ref1_), ref2(ref2_){};

  bool operator==(const WithCond& other) const {
    return ref1 == other.ref1 && ref2 == other.ref2;
  }
};
}  // namespace QE

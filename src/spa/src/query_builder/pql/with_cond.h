#include "query_builder/pql/attrref.h"

namespace QE {
class WithCond {
 public:
  AttrRef ref1;
  AttrRef ref2;
  WithCond(AttrRef ref1_, AttrRef ref2_);

  bool operator==(const WithCond& other) const;
};
}  // namespace QE

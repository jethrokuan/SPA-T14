#include <string>

namespace QE {
class Matcher {
 public:
  bool isPartial;
  std::string expr;

  Matcher(bool isPartial_, std::string expr_);

  bool operator==(const Matcher& other) const;
};

}  // namespace QE

#pragma once
#include <iostream>
#include <optional>

#include "query_evaluator/pql/relation.h"
#include "query_evaluator/pql/stmtentref.h"
#include "query_evaluator/pql/util.h"

namespace QE {
class SuchThat {
 private:
  Relation relation;
  StmtOrEntRef firstArg;
  StmtOrEntRef secondArg;
  SuchThat(Relation& r, StmtOrEntRef& a1, StmtOrEntRef& a2)
      : relation(r), firstArg(a1), secondArg(a2){};

 public:
  static std::optional<SuchThat> construct(Relation, StmtOrEntRef&,
                                           StmtOrEntRef&);
  static std::optional<SuchThat*> construct_heap(Relation, StmtOrEntRef&,
                                                 StmtOrEntRef&);
  Relation getRelation() const { return relation; }
  StmtOrEntRef getFirstArg() const { return firstArg; }
  StmtOrEntRef getSecondArg() const { return secondArg; }

  bool operator==(const SuchThat& st) const {
    return relation == st.relation && firstArg == st.firstArg &&
           secondArg == st.secondArg;
  }

  friend std::ostream& operator<<(std::ostream& os, SuchThat const& st) {
    os << getStringFromRelation(st.getRelation()) << "("
       << streamer{st.getFirstArg()} << ", " << streamer{st.getSecondArg()}
       << ")";
    return os;
  }
};
}  // namespace QE

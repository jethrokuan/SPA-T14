#pragma once
#include "query_builder/pql/ref.h"
#include "query_builder/pql/relation.h"

namespace QE {
class RelCond {
 private:
  //! Specializes Uses/Modifies to Uses(S|P) / Modifies(S|P)
  void specializeModifiesUses(std::vector<Declaration>* decls) {
    if (relation != Relation::Uses && relation != Relation::Modifies) return;
    // Special case for Modifies/Uses: specialize its type here
    // QuoteIdent ==> first argument entref ==> must be a procedure version
    if (std::holds_alternative<QuoteIdent>(arg1)) {
      relation =
          relation == Relation::Uses ? Relation::UsesP : Relation::ModifiesP;
    } else if (std::holds_alternative<StatementNumber>(arg1)) {
      // A statementnumber ==> this is a statement relation (*S)
      relation =
          relation == Relation::Uses ? Relation::UsesS : Relation::ModifiesS;
    } else if (auto syn = std::get_if<Synonym>(&arg1)) {
      // If it's a synonym - need to check if stmt synonym or proc
      // If no declaration present - will be caught in the validator
      if (auto decl = Declaration::findDeclarationForSynonym(decls, *syn)) {
        if (decl->getDesignEntity() == DesignEntity::PROCEDURE) {
          relation = relation == Relation::Uses ? Relation::UsesP
                                                : Relation::ModifiesP;
        } else {
          relation = relation == Relation::Uses ? Relation::UsesS
                                                : Relation::ModifiesS;
        }
      }
    }
  }

 public:
  Relation relation;
  Ref arg1;
  Ref arg2;
  RelCond(Relation relation_, Ref arg1_, Ref arg2_,
          std::vector<Declaration>* decls)
      : relation(relation_), arg1(arg1_), arg2(arg2_) {
    // Special case for Uses/Modifies (since have to infer specific relation)
    specializeModifiesUses(decls);
  };
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

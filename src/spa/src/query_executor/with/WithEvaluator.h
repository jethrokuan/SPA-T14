#pragma once
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_set>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/query_executor.h"

using namespace PKB;
using namespace QE;

class WithEvaluator {
 private:
  std::vector<QE::Declaration>* declarations;
  QE::WithCond* withCond;
  PKBManager* pkb;
  QueryConstraints& qc;

  QE::AttrRef argLeft;
  QE::AttrRef argRight;

  std::add_pointer_t<QE::QuoteIdent> argLeftAsQuoteIdent;
  std::add_pointer_t<unsigned int> argLeftAsNumber;
  std::add_pointer_t<QE::Synonym> argLeftAsSynonym;
  std::add_pointer_t<QE::SynAttr> argLeftAsSynAttr;
  std::add_pointer_t<QE::QuoteIdent> argRightAsQuoteIdent;
  std::add_pointer_t<unsigned int> argRightAsNumber;
  std::add_pointer_t<QE::Synonym> argRightAsSynonym;
  std::add_pointer_t<QE::SynAttr> argRightAsSynAttr;

  bool dispatch();

  bool handleBothArgsQuoteIdent();
  bool handleBothArgsNumber();
  bool handleBothArgsSynonym();
  bool handleNumberSynonym(unsigned int, QE::Synonym&);
  bool handleNumberSynAttr(unsigned int, QE::SynAttr&);
  bool handleQuoteIdentSynAttr(QE::QuoteIdent&, QE::SynAttr&);
  bool handleSynonymSynAttr(QE::Synonym&, QE::SynAttr&);

  //! /brief Convenience method to get an attribute value from a design entity
  //! value.
  //! E.g. for type print and value 3, we check 3.varName
  std::string applyAttrToDesignEntityValue(const DesignEntity& de_type,
                                           const std::string& val);

 public:
  WithEvaluator(std::vector<QE::Declaration>* decls, QE::WithCond* withCond,
                PKBManager* pkb, QueryConstraints& qc)
      : declarations(decls),
        withCond(withCond),
        pkb(pkb),
        qc(qc),
        argLeft(withCond->ref1),
        argRight(withCond->ref2){};

  //! External API to run the such-that query on the given PKB instance
  bool evaluate();
};
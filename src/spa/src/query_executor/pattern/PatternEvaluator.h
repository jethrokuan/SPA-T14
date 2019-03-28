#pragma once
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/query_constraints.h"
#include "query_executor/query_executor.h"

using namespace PKB;
using namespace QE;

class PatternEvaluator {
 protected:
  std::vector<QE::Declaration>* declarations;
  QE::PatternB* pattern;
  PKBManager* pkb;
  ConstraintDatabase& db;

  // Directly accessed values
  std::string synonym;
  Ref pattern_lhs;
  std::optional<Expression> pattern_rhs;

  // Derived expressions
  bool argLeftIsUnderscore = false;
  bool argLeftIsQuoteIdent = false;
  bool argLeftIsSynonym = false;
  bool argRightIsNull = false;
  bool argRightIsUnderscore = false;
  bool argRightIsMatcher = false;
  bool argRightIsPartialMatch = false;
  bool argRightIsCompleteMatch = false;
  std::optional<std::string> argLeftAsBasic;
  std::optional<Synonym> argLeftAsSynonym;
  std::optional<std::string> argRightAsExpression;

  //! Choose the correct handler to run for the pattern argument types found
  bool dispatch();

  bool handlePatternLHSUnderscore();
  bool handlePatternLHSQuoteIdent(const Synonym& syn, const std::string lhs,
                                  const QE::Expression& pattern_rhs);
  bool handlePatternLHSSynonym(const Synonym& syn, const Synonym& lhs,
                               const QE::Expression& pattern_rhs);

  bool dispatchPatternLHSUnderscoreRHSUnderscore();
  bool dispatchPatternLHSQuoteIdentRHSUnderscore();
  bool dispatchPatternLHSSynonymRHSUnderscore();
  bool dispatchPatternLHSUnderscoreRHSPartialMatch();
  bool dispatchPatternLHSQuoteIdentRHSPartialMatch();
  bool dispatchPatternLHSSynonymRHSPartialMatch();
  bool dispatchPatternLHSUnderscoreRHSCompleteMatch();
  bool dispatchPatternLHSQuoteIdentRHSCompleteMatch();
  bool dispatchPatternLHSSynonymRHSCompleteMatch();
  bool dispatchPatternLHSUnderscoreRHSNull();
  bool dispatchPatternLHSQuoteIdentRHSNull();
  bool dispatchPatternLHSSynonymRHSNull();

  virtual std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSUnderscore() = 0;
  virtual std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSUnderscore() = 0;
  virtual PairedConstraintSet handlePatternLHSSynonymRHSUnderscore() = 0;
  virtual std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSPartialMatch() = 0;
  virtual std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSPartialMatch() = 0;
  virtual std::optional<PairedConstraintSet>
  handlePatternLHSSynonymRHSPartialMatch() = 0;
  virtual std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSCompleteMatch() = 0;
  virtual std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSCompleteMatch() = 0;
  virtual std::optional<PairedConstraintSet>
  handlePatternLHSSynonymRHSCompleteMatch() = 0;
  virtual std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSNull() = 0;
  virtual std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSNull() = 0;
  virtual std::optional<PairedConstraintSet>
  handlePatternLHSSynonymRHSNull() = 0;

 public:
  PatternEvaluator(std::vector<QE::Declaration>* decls, QE::PatternB* pattern,
                   PKBManager* pkb, ConstraintDatabase& db)
      : declarations(decls),
        pattern(pattern),
        pkb(pkb),
        db(db),
        synonym(pattern->getSynonym().synonym),
        pattern_lhs(pattern->getFirstArg()),
        pattern_rhs(pattern->getSecondArg()){};

  //! External API to run the pattern query on the given PKB instance
  bool evaluate();
};
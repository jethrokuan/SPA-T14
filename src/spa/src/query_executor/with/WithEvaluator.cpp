#include "query_executor/with/WithEvaluator.h"
#include <cassert>
#include <string>
#include <unordered_set>
#include <vector>

#include "query_executor/query_executor.h"

bool WithEvaluator::evaluate() {
  argLeftAsQuoteIdent = std::get_if<QuoteIdent>(&argLeft.attr);
  argLeftAsNumber = std::get_if<unsigned int>(&argLeft.attr);
  argLeftAsSynonym = std::get_if<Synonym>(&argLeft.attr);
  argLeftAsSynAttr = std::get_if<SynAttr>(&argLeft.attr);
  argRightAsQuoteIdent = std::get_if<QuoteIdent>(&argRight.attr);
  argRightAsNumber = std::get_if<unsigned int>(&argRight.attr);
  argRightAsSynonym = std::get_if<Synonym>(&argRight.attr);
  argRightAsSynAttr = std::get_if<SynAttr>(&argRight.attr);

  return dispatch();
}

bool WithEvaluator::dispatch() {
  if (argLeftAsQuoteIdent && argRightAsQuoteIdent) {
    return handleBothArgsQuoteIdent();
  } else if (argLeftAsNumber && argRightAsNumber) {
    return handleBothArgsNumber();
  } else if (argLeftAsSynonym && argRightAsSynonym) {
    return handleBothArgsSynonym();
  } else if (argLeftAsSynAttr && argRightAsSynAttr) {
    return handleBothArgsSynAttr();
  } else if (argLeftAsNumber && argRightAsSynonym) {
    return handleNumberSynonym(*argLeftAsNumber, *argRightAsSynonym);
  } else if (argRightAsNumber && argLeftAsSynonym) {
    return handleNumberSynonym(*argRightAsNumber, *argLeftAsSynonym);
  } else if (argLeftAsNumber && argRightAsSynAttr) {
    return handleNumberSynAttr(*argLeftAsNumber, *argRightAsSynAttr);
  } else if (argRightAsNumber && argLeftAsSynAttr) {
    return handleNumberSynAttr(*argRightAsNumber, *argLeftAsSynAttr);
  } else if (argLeftAsQuoteIdent && argRightAsSynAttr) {
    return handleQuoteIdentSynAttr(*argLeftAsQuoteIdent, *argRightAsSynAttr);
  } else if (argRightAsQuoteIdent && argLeftAsSynAttr) {
    return handleQuoteIdentSynAttr(*argRightAsQuoteIdent, *argLeftAsSynAttr);
  } else if (argLeftAsSynonym && argRightAsSynAttr) {
    return handleSynonymSynAttr(*argLeftAsSynonym, *argRightAsSynAttr);
  } else if (argRightAsSynonym && argLeftAsSynAttr) {
    return handleSynonymSynAttr(*argRightAsSynonym, *argLeftAsSynAttr);
  } else {
    return false;
  }
}

bool WithEvaluator::handleBothArgsQuoteIdent() {
  // with "asdf" = "asdf"
  return (*argLeftAsQuoteIdent == *argRightAsQuoteIdent);
}

bool WithEvaluator::handleBothArgsNumber() {
  // with 2 = 3
  return (*argLeftAsNumber == *argRightAsNumber);
}

bool WithEvaluator::handleBothArgsSynonym() {
  // with n1 = n2, where both n1, n2 are prog_line
  // prog_line n1, n2; Select <n1, n2> with n1 = n2
  // Only allow values of n1 and n2 where n1 = n2
  // Our simplified case: {(n', n') | n' in prog_line}
  auto all_prog_lines = QueryExecutor::getSelect(pkb, DesignEntity::PROG_LINE);
  // [1,2,3,4] --> {(1,1), (2,2), (3,3), (4,4)}
  PairedConstraintSet pcs;
  for (const auto& prog_line : all_prog_lines) {
    pcs.insert({prog_line, prog_line});
  }
  db.addToPairedVariableConstraints(argLeftAsSynonym->synonym,
                                    argRightAsSynonym->synonym, pcs);
  return true;
}

bool WithEvaluator::handleBothArgsSynAttr() {
  // Get all possible values for both synonyms involved
  auto synAttr1 = *argLeftAsSynAttr;
  auto synAttr2 = *argRightAsSynAttr;
  auto found_declaration_1 =
      Declaration::findDeclarationForSynonym(declarations, synAttr1.synonym)
          .value();
  auto found_declaration_2 =
      Declaration::findDeclarationForSynonym(declarations, synAttr2.synonym)
          .value();

  auto design_entity_type_1 = found_declaration_1.getDesignEntity();
  auto design_entity_type_2 = found_declaration_2.getDesignEntity();
  auto all_des_1 = QueryExecutor::getSelect(pkb, design_entity_type_1);
  auto all_des_2 = QueryExecutor::getSelect(pkb, design_entity_type_2);

  PairedConstraintSet equal_values;
  for (const auto& de1 : all_des_1) {
    for (const auto& de2 : all_des_2) {
      auto de1_applied_attrname = QueryExecutor::applyAttrToDesignEntityValue(
          pkb, design_entity_type_1, de1, synAttr1.attrName);
      auto de2_applied_attrname = QueryExecutor::applyAttrToDesignEntityValue(
          pkb, design_entity_type_2, de2, synAttr2.attrName);
      if (de1_applied_attrname == de2_applied_attrname) {
        equal_values.insert({de1, de2});
      }
    }
  }

  return db.addToPairedVariableConstraints(
      synAttr1.synonym.synonym, synAttr2.synonym.synonym, equal_values);
}

bool WithEvaluator::handleNumberSynonym(unsigned int num, Synonym& synonym) {
  return db.addToSingleVariableConstraints(
      synonym.synonym, SingleConstraintSet{std::to_string(num)});
}

bool WithEvaluator::handleNumberSynAttr(unsigned int num, SynAttr& synAttr) {
  // For all Number <-> SynAttr constraints: we don't need to do any extra work
  // All the attribute applications are no-ops
  // We MUST have accessed constant.value / <stmttype>.stmt# - no work to do
  // Can just add constraint: (c) -> (10) for e.g.
  return db.addToSingleVariableConstraints(
      synAttr.synonym.synonym, SingleConstraintSet{std::to_string(num)});
}

bool WithEvaluator::handleQuoteIdentSynAttr(QuoteIdent& quoteIdent,
                                            SynAttr& synAttr) {
  // General algorithm: get all design entities for this synonym
  // Then, filter by a generic operation that applies the Attr on the synonym
  // and checks it against the quoteIdent passed in
  auto found_declaration =
      Declaration::findDeclarationForSynonym(declarations, synAttr.synonym)
          .value();
  auto design_entity_type = found_declaration.getDesignEntity();
  // If we have a variable or procedure: no - op, just constrain directly
  if (design_entity_type == DesignEntity::PROCEDURE ||
      design_entity_type == DesignEntity::VARIABLE) {
    return db.addToSingleVariableConstraints(
        synAttr.synonym.synonym, SingleConstraintSet{quoteIdent.quote_ident});
  } else {
    auto all_des = QueryExecutor::getSelect(pkb, design_entity_type);
    // Filter out values that don't match call to .procName/.varName for
    // call/read/print
    SingleConstraintSet filtered_des;
    std::copy_if(all_des.begin(), all_des.end(),
                 std::inserter(filtered_des, filtered_des.end()),
                 [&, this](const std::string de) {
                   return QueryExecutor::applyAttrToDesignEntityValue(
                              pkb, design_entity_type, de) ==
                          quoteIdent.quote_ident;
                 });
    return db.addToSingleVariableConstraints(synAttr.synonym.synonym,
                                             filtered_des);
  }
}

bool WithEvaluator::handleSynonymSynAttr(Synonym& synonym, SynAttr& synAttr) {
  // General algorithm: get all design entities for this synonym
  // Then, filter by a generic operation that applies the Attr on the synonym
  // and checks it against the quoteIdent passed in
  auto found_declaration =
      Declaration::findDeclarationForSynonym(declarations, synAttr.synonym)
          .value();
  auto design_entity_type = found_declaration.getDesignEntity();
  auto all_prog_lines = QueryExecutor::getSelect(pkb, DesignEntity::PROG_LINE);
  auto all_des = QueryExecutor::getSelect(pkb, design_entity_type);
  if (all_des.empty()) return false;

  // Perform set intersection and duplicate all resultant values
  // E.g. (1, 2) INT (2, 3) ==> {(2, 2)}
  return db.addToPairedVariableConstraints(
      synonym.synonym, synAttr.synonym.synonym,
      Utils::unorderedSetIntersectionToPairedSet(all_prog_lines, all_des));
}
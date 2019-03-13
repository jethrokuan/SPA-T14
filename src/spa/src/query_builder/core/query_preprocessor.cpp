#include <algorithm>
#include <iostream>
#include <sstream>
#include <variant>
#include "query_builder/core/exceptions.h"
#include "query_builder/core/query_preprocessor.h"
#include "query_builder/core/query_tokenizer.h"
#include "query_builder/pql/pql.h"

using namespace QE;

Query* QueryPreprocessor::getQuery(std::string& pql_query_string) {
  // If empty query - throw error
  if (pql_query_string.empty()) {
    throw PQLTokenizeException("Query string is empty.");
  }

  // Tokenize the query string into a struct containing vector of strings
  auto query_tokenizer = QueryTokenizer();
  QueryTokenizerTokens query_tokens =
      query_tokenizer.getTokens(pql_query_string);

  // Begin to fill up query object with components of query
  auto query = new Query();
  QueryPreprocessor::parseDeclarations(query, query_tokens.declaration_tokens);
  QueryPreprocessor::parseSelect(query, query_tokens.select_tokens);
  QueryPreprocessor::parseSuchThat(query, query_tokens.such_that_tokens);
  QueryPreprocessor::parsePattern(query, query_tokens.pattern_tokens);

  return query;
}

void QueryPreprocessor::parseDeclarations(
    Query* query, std::vector<std::string>* declaration_tokens) {
  // Default value for de
  auto de = DesignEntity::ASSIGN;
  auto decls = new std::vector<Declaration>();
  bool parsing_declaration = true;
  for (size_t i = 0; i < declaration_tokens->size(); i++) {
    if (parsing_declaration) {
      // This might throw an error - it will terminate this process
      de = getDesignEntity((*declaration_tokens)[i]);
      parsing_declaration = false;
    } else if ((*declaration_tokens)[i] ==
               QueryTokenizer::DECLARATION_DUMMY_SEPARATOR) {
      // Move to parsing next design entity type
      parsing_declaration = true;
    } else {
      // This is the synonym half of the decl: store the DE we have into our
      // list
      auto synonym = (*declaration_tokens)[i];
      // Test whether this synonym is valid
      std::optional<Synonym> synonymObj = Synonym::construct(synonym);
      if (!synonymObj) {
        // Failure in synonym construction: regex invalud
        std::string ex_str;
        // This is faster than stringstream construction
        ex_str = "Could not parse synonym " + synonym +
                 " when processing design entity " + getDesignEntityString(de) +
                 "\n";
        throw PQLParseException(ex_str);
      }

      decls->push_back(Declaration(de, synonymObj.value()));
    }
  }
  query->declarations = decls;
}

void QueryPreprocessor::parseSelect(Query* query,
                                    std::vector<std::string>* select_tokens) {
  if (select_tokens->size() != 2) {
    throw PQLParseException(
        "Invalid number of tokens found for Select statement. Expected 2, "
        "found " +
        std::to_string(select_tokens->size()));
  }

  // The synonym in the Select statement (e.g. {"Select", "p"} --> "p")
  std::string synonym_to_match = select_tokens->at(1);

  // Search declarations to find one that matches this synyonm
  // THIS CAN THROW AN EXCEPTION - we do not catch
  query->result->T = ResultType::TUPLE;
  query->result->selected_declarations =
      new std::vector{findDeclaration(query->declarations, synonym_to_match)};
}

void QueryPreprocessor::parseSuchThat(
    Query* query, std::vector<std::string>* such_that_tokens) {
  if (such_that_tokens == nullptr) {
    return;
  } else if (such_that_tokens->size() <= 2) {
    throw PQLTokenizeException(
        "Incorrect number of such that tokens found, expecting >2, saw " +
        std::to_string(such_that_tokens->size()));
  }

  // Join and all such-that tokens excluding "such" and "that"
  std::string joined_such_that;
  for (auto it = such_that_tokens->begin() + 2; it != such_that_tokens->end();
       ++it) {
    joined_such_that += *it;
  }

  // Erase all whitespace
  std::string::iterator end_pos =
      std::remove(joined_such_that.begin(), joined_such_that.end(), ' ');
  joined_such_that.erase(end_pos, joined_such_that.end());
  end_pos = std::remove(joined_such_that.begin(), joined_such_that.end(), '\t');
  joined_such_that.erase(end_pos, joined_such_that.end());

  // For a Relation to match: it must match 'RelationName'+'(' since some
  // relations are substrings of other relations
  const auto& relationToStringMap = getRelationToStringMap();
  size_t found_end_idx = 0;
  bool found = false;
  Relation relation;
  for (const auto& r : relationToStringMap) {
    auto string_to_match = r.second + '(';
    auto found_start_idx = joined_such_that.find(string_to_match);
    if (found_start_idx != std::string::npos) {
      // Set the relation we found - args set later
      // TODO: It also needs to be found RIGHT AFTER the such that
      relation = r.first;
      found_end_idx = found_start_idx + r.second.length();
      found = true;
      break;
    }
  }

  if (!found) {
    throw PQLParseException(
        "Cannot find a valid relation in such that statement: " +
        joined_such_that);
  }

  auto arg1_start_idx = found_end_idx + 1;
  auto arg1_len = joined_such_that.find(',') - arg1_start_idx;
  auto arg2_start_idx = joined_such_that.find(',') + 1;
  auto arg2_len = joined_such_that.find(')') - arg2_start_idx;

  // These are the two strings representing each argument to SuchThat
  auto arg1 = joined_such_that.substr(arg1_start_idx, arg1_len);
  auto arg2 = joined_such_that.substr(arg2_start_idx, arg2_len);

  // Parse arguments to get either entRef or stmtRef
  auto arg_types = getArgTypesFromRelation(relation);

  auto stmt_or_entref_1 = argToStmtOrEntRef(arg1, arg_types.first);
  auto stmt_or_entref_2 = argToStmtOrEntRef(arg2, arg_types.second);

  if (!stmt_or_entref_1) {
    throw PQLParseException(
        "Cannot parse argument " + arg1 + " as a(n) " +
        refTypeToString(arg_types.first) +
        ", which is required for first argument of relation type " +
        getStringFromRelation(relation));
  }
  if (!stmt_or_entref_2) {
    throw PQLParseException(
        "Cannot parse argument " + arg2 + " as a(n) " +
        refTypeToString(arg_types.second) +
        ", which is required for second argument of relation type " +
        getStringFromRelation(relation));
  }

  // Construct final Such That relation
  auto opt_suchthat = SuchThat::construct_heap(
      relation, stmt_or_entref_1.value(), stmt_or_entref_2.value());
  if (opt_suchthat) {
    query->such_that = new std::vector{opt_suchthat.value()};
  } else {
    throw PQLParseException(
        "Such That parse error - did not get correct stmtref/entref "
        "combination for given relation (this error message should not be seen "
        "- should be caught be earlier block");
  }
}

void QueryPreprocessor::parsePattern(Query* query,
                                     std::vector<std::string>* pattern_tokens) {
  if (pattern_tokens == nullptr) {
    return;
  } else if (pattern_tokens->size() <= 1) {
    throw PQLTokenizeException(
        "Expected at least 1 pattern token in pattern clause, got " +
        std::to_string(pattern_tokens->size()));
  }

  // Join and all such-that tokens excluding "pattern"
  std::string joined_pattern;
  for (auto it = pattern_tokens->begin() + 1; it != pattern_tokens->end();
       ++it) {
    joined_pattern += *it;
  }

  // Erase all whitespace
  // May the programming gods have mercy on my soul
  std::string::iterator end_pos =
      std::remove(joined_pattern.begin(), joined_pattern.end(), ' ');
  joined_pattern.erase(end_pos, joined_pattern.end());
  end_pos = std::remove(joined_pattern.begin(), joined_pattern.end(), '\t');
  joined_pattern.erase(end_pos, joined_pattern.end());

  auto syn_assign_len = joined_pattern.find('(');

  auto arg1_start_idx = joined_pattern.find('(') + 1;
  auto arg1_len = joined_pattern.find(',') - arg1_start_idx;
  auto arg2_start_idx = joined_pattern.find(',') + 1;
  auto arg2_len = joined_pattern.find(')') - arg2_start_idx;

  auto syn_assign = joined_pattern.substr(0, syn_assign_len);
  auto arg1 = joined_pattern.substr(arg1_start_idx, arg1_len);
  auto arg2 = joined_pattern.substr(arg2_start_idx, arg2_len);

  auto synonym = Synonym::construct(syn_assign);
  auto entref = argToEntRef(arg1);
  auto exprspec = argToExprSpec(arg2);

  if (!synonym) {
    throw PQLParseException("Pattern parse error - cannot parse arg " +
                            syn_assign + " as synonym");
  }
  if (!entref) {
    throw PQLParseException("Pattern parse error - cannot parse arg " + arg1 +
                            " as entref");
  }
  if (!exprspec) {
    throw PQLParseException("Pattern parse error - cannot parse arg " + arg2 +
                            " as exprspec");
  }

  // Construct final Pattern relation
  auto opt_pattern = Pattern::construct_heap(synonym.value(), entref.value(),
                                             exprspec.value());
  if (opt_pattern) {
    query->pattern = new std::vector{opt_pattern.value()};
  } else {
    throw PQLParseException(
        "Pattern parse error - could not construct final pattern from "
        "individually valid parts.");
  }
}

// Find the first declaration that matches this synonym
Declaration* QueryPreprocessor::findDeclaration(
    std::vector<Declaration>* declarations,
    const std::string synonym_to_match) {
  auto found_declaration = std::find_if(
      declarations->begin(), declarations->end(),
      [&](auto decl) { return decl.getSynonym().synonym == synonym_to_match; });

  if (found_declaration == declarations->end()) {
    throw PQLParseException("Semantic Error: cannot match synonym " +
                            synonym_to_match +
                            " to list of declarations given");
  }

  return &declarations->at(
      std::distance(declarations->begin(), found_declaration));
}

// Tries to produce either a statement or entity reference from the given string
std::optional<StmtOrEntRef> QueryPreprocessor::argToStmtOrEntRef(
    std::string arg, RefType refType) {
  if (refType == RefType::STMTREF) {
    auto ret = argToStmtRef(arg);
    if (ret) {
      return ret.value();
    } else {
      return std::nullopt;
    }
  } else {
    auto ret = argToEntRef(arg);
    if (ret) {
      return ret.value();
    } else {
      return std::nullopt;
    }
  }
}

std::optional<StmtRef> QueryPreprocessor::argToStmtRef(std::string arg) {
  StmtRef s1 = QE::Underscore();
  std::optional<Synonym> opt_synonym;
  if (arg == "_") {
    s1 = QE::Underscore();
  } else if (has_only_digits(arg)) {
    s1 = std::stoul(arg);
  } else if ((opt_synonym = Synonym::construct(arg))) {
    s1 = opt_synonym.value();
  } else {
    return std::nullopt;
  }
  return s1;
}

std::optional<EntRef> QueryPreprocessor::argToEntRef(std::string arg) {
  QE::EntRef s1 = QE::Underscore();
  std::optional<Synonym> opt_synonym;
  std::optional<QuoteIdent> opt_quoteident;
  if (arg == "_") {
    s1 = QE::Underscore();
  } else if ((opt_synonym = Synonym::construct(arg))) {
    s1 = opt_synonym.value();
  } else if ((opt_quoteident = QuoteIdent::construct(arg))) {
    s1 = opt_quoteident.value();
  } else {
    return std::nullopt;
  }
  return s1;
}

std::optional<ExpressionSpec> QueryPreprocessor::argToExprSpec(
    std::string arg) {
  auto duf = DoubleUnderscoreFactor::construct(arg);
  if (duf) {
    return duf;
  } else if (arg == "_") {
    return Underscore();
  } else {
    return std::nullopt;
  }
}

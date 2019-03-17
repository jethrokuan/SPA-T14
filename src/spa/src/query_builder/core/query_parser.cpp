#include "query_builder/core/exceptions.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/pql/attrref.h"
#include "query_builder/pql/design_entity.h"
#include "query_builder/pql/query.h"
#include "query_builder/pql/ref.h"
#include "query_builder/pql/result.h"
#include "query_builder/pql/underscore.h"
#include "utils/utils.h"

#include <optional>

using namespace QE;

QueryParser::QueryParser(std::vector<std::string> tokens)
    : query_(new Query()), current_(0), tokens_(tokens) {}

bool QueryParser::match(std::string s) {
  if (check(s)) {
    advance();
    return true;
  }
  return false;
}

bool QueryParser::check(std::string s) {
  if (isAtEnd()) return false;
  return peek().compare(s) == 0;
}

bool QueryParser::expect(std::string s) {
  if (match(s)) {
    return true;
  }
  throw PQLParseException("Expected '" + s + "', got '" + peek() + "'.");
}

bool QueryParser::isAtEnd() { return peek().compare("") == 0; }
std::string QueryParser::peek() { return tokens_[current_]; }
std::string QueryParser::previous() { return tokens_[current_ - 1]; }
std::string QueryParser::advance() {
  if (!isAtEnd()) current_++;
  return previous();
}

Declaration* QueryParser::findDeclaration(const Synonym synonym) {
  auto found_declaration =
      std::find_if(query_->declarations->begin(), query_->declarations->end(),
                   [&](auto decl) { return decl.getSynonym() == synonym; });

  if (found_declaration == query_->declarations->end()) {
    throw PQLValidationException("Semantic Error: cannot match synonym " +
                                 synonym.synonym +
                                 " to list of declarations given");
  }

  return &query_->declarations->at(
      std::distance(query_->declarations->begin(), found_declaration));
}

bool QueryParser::parseDeclarationClause() {
  unsigned int save_loc = current_;
  std::vector<Synonym> synonyms;
  std::string de_str = advance();
  std::string synonym_str = advance();
  // TODO: Inheriting clunky code here, should fix someday
  DesignEntity de;
  try {
    de = getDesignEntity(de_str);
  } catch (const PQLParseException& e) {
    current_ = save_loc;
    return false;
  }
  std::optional<Synonym> synonym = Synonym::construct(synonym_str);
  if (!synonym) {
    throw PQLParseException("Failed to parse synonym");
  }
  synonyms.push_back(*synonym);

  while (!match(";")) {
    expect(",");
    synonym_str = advance();
    synonym = Synonym::construct(synonym_str);
    if (!synonym) {
      throw PQLParseException("Failed to parse synonym");
    }
    synonyms.push_back(*synonym);
  }

  for (const Synonym syn : synonyms) {
    Declaration decl = Declaration(de, syn);
    query_->declarations->push_back(decl);
  }

  return true;
}

ResultItem QueryParser::parseResultItem() {
  auto synonym_str = advance();
  auto synonym = Synonym::construct(synonym_str);
  std::variant<Synonym, SynAttr> result_item;

  if (!synonym) {
    throw PQLParseException("Expected a synonym, got " + previous());
  }

  if (match(".")) {
    AttrName name = attrNameFromString(advance());
    auto syn_attr = SynAttr::construct(*synonym, name, query_->declarations);
    if (!syn_attr) {
      throw PQLParseException("Invalid synonym - attrName pair: (" +
                              synonym->synonym + ", " + previous() + ")");
    }
    result_item = syn_attr.value();
  } else {
    result_item = synonym.value();
  }

  return result_item;
}

void QueryParser::parseResult() {
  std::vector<Synonym> synonyms;
  query_->result->T = ResultType::TUPLE;
  query_->result->selected_declarations = new std::vector<ResultItem>();

  if (match("BOOLEAN")) {
    query_->result->T = ResultType::BOOLEAN;
    return;
  }

  if (!match("<")) {
    auto result_item = parseResultItem();
    query_->result->selected_declarations->push_back(result_item);
  } else {
    while (!match(">")) {
      auto result_item = parseResultItem();
      query_->result->selected_declarations->push_back(result_item);

      if (match(">")) {
        break;
      } else {
        expect(",");
      }
    }
  }
};

Ref QueryParser::parseRef() {
  if (match("_")) {
    return QE::Underscore();
  } else if (has_only_digits(peek())) {
    return atoi(advance().c_str());
  } else if (match("\"")) {
    std::string ident = advance();
    expect("\"");
    return QuoteIdent(ident);
  } else if (is_valid_synonym(peek())) {
    std::string synonym_str = advance();
    auto synonym = Synonym::construct(synonym_str);
    return synonym.value();
  } else {
    throw PQLParseException("Expecting a ref, got '" + peek() + "'.");
  }
}

void QueryParser::parseRelRef() {
  Relation relation;
  if (match("Modifies")) {
    relation = Relation::Modifies;
  } else if (match("Uses")) {
    relation = Relation::Uses;
  } else if (match("Calls")) {
    if (match("*")) {
      relation = Relation::CallsT;
    } else {
      relation = Relation::Calls;
    }
  } else if (match("Parent")) {
    if (match("*")) {
      relation = Relation::ParentT;
    } else {
      relation = Relation::Parent;
    }
  } else if (match("Follows")) {
    if (match("*")) {
      relation = Relation::FollowsT;
    } else {
      relation = Relation::Follows;
    }
  } else if (match("Next")) {
    if (match("*")) {
      relation = Relation::NextT;
    } else {
      relation = Relation::Next;
    }
  } else if (match("Affects")) {
    if (match("*")) {
      relation = Relation::AffectsT;
    } else {
      relation = Relation::Affects;
    }
  } else {
    throw PQLParseException("Unknown relation: " + peek());
  }

  expect("(");
  auto ref_1 = parseRef();
  expect(",");
  auto ref_2 = parseRef();
  expect(")");

  // Declarations passed to RelCond to do specialization for Modifies/Uses
  RelCond* relcond = new RelCond(relation, ref_1, ref_2, query_->declarations);
  query_->rel_cond->push_back(relcond);
}

void QueryParser::parseRelCond() {
  parseRelRef();
  while (match("and")) {
    parseRelRef();
  }
}

bool QueryParser::parseSuchThat() {
  unsigned int save_loc = current_;

  if (!match("such")) {
    current_ = save_loc;
    return false;
  }

  expect("that");

  parseRelCond();
  return true;
}

Expression QueryParser::parseExpression() {
  bool isPartial = true;
  std::string expr = "";

  if (match("\"")) {
    expr = advance();
    isPartial = false;
    expect("\"");
  } else {
    expect("_");
    if (peek().compare(")") == 0) {
      return Underscore();
    } else {
      expect("\"");
      expr = advance();
      expect("\"");
      expect("_");
    }
  }

  return Matcher(isPartial, expr);
}

bool QueryParser::parsePattern() {
  if (!match("pattern")) {
    return false;
  }

  auto synonym_str = advance();
  auto synonym = Synonym::construct(synonym_str);

  if (!synonym) {
    throw PQLParseException("Expected a synonym, got " + previous());
  }

  auto decl = findDeclaration(synonym.value());

  expect("(");

  PatternB* pattern;
  Ref ref = Underscore();  // Default, will be overriden
  Expression expr;

  switch (decl->getDesignEntity()) {
    case DesignEntity::ASSIGN:
      ref = parseRef();
      expect(",");
      expr = parseExpression();
      pattern = new PatternB(synonym.value(), ref, expr);
      break;
    case DesignEntity::IF:
      ref = parseRef();
      expect(",");
      expect("_");
      expect(",");
      expect("_");
      pattern = new PatternB(synonym.value(), ref);
      break;
    case DesignEntity::WHILE:
      ref = parseRef();
      expect(",");
      expect("_");
      pattern = new PatternB(synonym.value(), ref);
      break;
    case DesignEntity::READ:
    case DesignEntity::STMT:
    case DesignEntity::PRINT:
    case DesignEntity::CALL:
    case DesignEntity::VARIABLE:
    case DesignEntity::CONSTANT:
    case DesignEntity::PROCEDURE:
    case DesignEntity::PROG_LINE:
      throw PQLParseException("pattern clause not supported for " +
                              getDesignEntityString(decl->getDesignEntity()));
  }

  expect(")");

  query_->patternb->push_back(pattern);

  return true;
}

AttrRef QueryParser::parseAttrRef() {
  std::optional<AttrRef> attr_ref = std::nullopt;
  if (has_only_digits(peek())) {
    unsigned int val = std::stoi(advance());
    attr_ref = AttrRef::construct(val, query_->declarations);
  } else if (match("\"")) {
    std::string ident_str = advance();
    QuoteIdent ident_ = QuoteIdent(ident_str);
    expect("\"");
    attr_ref = AttrRef::construct(ident_, query_->declarations);
  } else if (is_valid_synonym(peek())) {
    std::string synonym_str = advance();
    auto synonym = Synonym::construct(synonym_str);

    if (match(".")) {  // is SynAttr
      AttrName name = attrNameFromString(advance());
      auto syn_attr = SynAttr::construct(*synonym, name, query_->declarations);
      if (!syn_attr) {
        throw PQLParseException("Invalid synonym - attrName pair: (" +
                                synonym->synonym + ", " + previous() + ")");
      }
      attr_ref = AttrRef::construct(*syn_attr, query_->declarations);
    } else {
      attr_ref = AttrRef::construct(*synonym, query_->declarations);
    }
  } else {
    attr_ref = std::nullopt;
  }

  if (!attr_ref) {
    throw PQLParseException("Couldn't parse attrref.");
  }

  return attr_ref.value();
}

void QueryParser::parseAttrCompare() {
  AttrRef ref1 = parseAttrRef();
  expect("=");
  AttrRef ref2 = parseAttrRef();
  WithCond* with_cond = new WithCond(ref1, ref2);
  query_->with_cond->push_back(with_cond);
}

void QueryParser::parseAttrCond() {
  parseAttrCompare();
  while (match("and")) {
    parseAttrCompare();
  }
}

bool QueryParser::parseWith() {
  if (!match("with")) {
    return false;
  }

  parseAttrCond();
  return true;
}

Query QueryParser::parse() {
  // Parsing declarations
  while (!isAtEnd()) {
    while (!isAtEnd()) {
      bool isDeclarationClause = parseDeclarationClause();
      if (!isDeclarationClause) {
        break;
      }
    }

    expect("Select");

    parseResult();

    while (!isAtEnd()) {
      if (parseSuchThat()) continue;
      if (parsePattern()) continue;
      if (parseWith()) continue;
      throw PQLParseException(
          "Expecting a such-that, pattern or with clause., got " + peek());
    }
  }

  return *query_;
  // Parsing selects and clauses
}

#include "structs/node.h"

#include <iostream>

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...)->overload<Ts...>;

StmtListNode::StmtListNode(std::vector<StmtNode> stmtList)
    : StmtList(std::move(stmtList)){};
bool StmtListNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const StmtListNode*>(&other);
  return casted_other != 0 &&
         this->StmtList.size() == casted_other->StmtList.size() &&
         std::equal(begin(this->StmtList), end(this->StmtList),
                    begin(casted_other->StmtList), end(casted_other->StmtList),
                    [](const auto t, const auto o) {
                      return std::visit(overload{[](auto& tp, auto& op) {
                                          return *tp == *op;
                                        }},
                                        t, o);
                    });
};

NumberNode::NumberNode(const std::string val) : Val(val){};
bool NumberNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const NumberNode*>(&other);
  return casted_other != 0 && this->Val.compare(casted_other->Val) == 0;
};

VariableNode::VariableNode(const std::string name) : Name(name){};
bool VariableNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const VariableNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0;
};

ReadNode::ReadNode(std::shared_ptr<VariableNode> var) : Var(std::move(var)){};
bool ReadNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ReadNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

PrintNode::PrintNode(std::shared_ptr<VariableNode> var) : Var(std::move(var)){};
bool PrintNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const PrintNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

ProcedureNode::ProcedureNode(const std::string name,
                             std::shared_ptr<StmtListNode> stmtList)
    : Name(name), StmtList(std::move(stmtList)){};
bool ProcedureNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ProcedureNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0 &&
         *this->StmtList == *casted_other->StmtList;
};

TermPNode::TermPNode(FactorNode factor, std::string op,
                     std::shared_ptr<TermPNode> termP)
    : Factor(factor), Op(op), TermP(std::move(termP)){};

bool TermPNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const TermPNode*>(&other);
  return casted_other != 0 && this->Op.compare(casted_other->Op) == 0 &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Factor,
                    casted_other->Factor);
};

BinOpNode::BinOpNode(ExprNode left, ExprNode right, std::string op)
    : Left(std::move(left)), Right(std::move(right)), Op(op){};
bool BinOpNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const BinOpNode*>(&other);
  return casted_other != 0 &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Left,
                    casted_other->Left) &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Right,
                    casted_other->Right);
};

TermNode::TermNode(FactorNode factor, std::shared_ptr<TermPNode> termP)
    : Factor(factor), TermP(std::move(termP)){};
TermNode::TermNode(FactorNode factor) : Factor(factor), TermP(nullptr){};
bool TermNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const TermNode*>(&other);
  return casted_other != 0 &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Factor,
                    casted_other->Factor) &&
         (this->TermP == casted_other->TermP ||
          *this->TermP == *casted_other->TermP);
}

ExprPNode::ExprPNode(std::shared_ptr<TermNode> term, std::string op)
    : Term(std::move(term)), Op(op){};

ExprPNode::ExprPNode(std::shared_ptr<TermNode> term, std::string op,
                     std::shared_ptr<ExprPNode> exprP)
    : Term(std::move(term)), Op(op), ExprP(std::move(exprP)){};

bool ExprPNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ExprPNode*>(&other);
  return casted_other != 0 && *this->Term == *casted_other->Term &&
         (this->ExprP == casted_other->ExprP ||
          *this->ExprP == *casted_other->ExprP) &&
         this->Op.compare(casted_other->Op) == 0;
};

AssignNode::AssignNode(std::shared_ptr<VariableNode> var, ExprNode expr)
    : Var(std::move(var)), Expr(expr){};
bool AssignNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const AssignNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Expr,
                    casted_other->Expr);
};

RelExprNode::RelExprNode(RelFactorNode lhs, std::string op, RelFactorNode rhs)
    : LHS(lhs), Op(op), RHS(rhs){};
bool RelExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const RelExprNode*>(&other);
  return casted_other != 0 &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->LHS,
                    casted_other->LHS) &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->RHS,
                    casted_other->RHS) &&
         this->Op.compare(casted_other->Op) == 0;
};

CondExprNode::CondExprNode(std::shared_ptr<RelExprNode> relExpr)
    : RelExpr(std::move(relExpr)){};
CondExprNode::CondExprNode(std::shared_ptr<CondExprNode> condLHS)
    : CondLHS(std::move(condLHS)), Op("!"){};
CondExprNode::CondExprNode(std::shared_ptr<CondExprNode> condLHS,
                           std::string op,
                           std::shared_ptr<CondExprNode> condRHS)
    : CondLHS(std::move(condLHS)), Op(op), CondRHS(std::move(condRHS)){};
bool CondExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const CondExprNode*>(&other);
  return casted_other != 0 &&
         (this->RelExpr == casted_other->RelExpr ||
          *this->RelExpr == *casted_other->RelExpr) &&
         (this->CondLHS == casted_other->CondLHS ||
          *this->CondLHS == *casted_other->CondLHS) &&
         (this->CondRHS == casted_other->CondRHS ||
          *this->CondRHS == *casted_other->CondRHS) &&
         (this->Op.compare(casted_other->Op) == 0);
};

WhileNode::WhileNode(std::shared_ptr<CondExprNode> condExpr,
                     std::shared_ptr<StmtListNode> stmtList)
    : CondExpr(std::move(condExpr)), StmtList(std::move(stmtList)){};
bool WhileNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const WhileNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         *this->StmtList == *casted_other->StmtList;
};

IfNode::IfNode(std::shared_ptr<CondExprNode> condExpr,
               std::shared_ptr<StmtListNode> stmtListThen,
               std::shared_ptr<StmtListNode> stmtListElse)
    : CondExpr(std::move(condExpr)),
      StmtListThen(std::move(stmtListThen)),
      StmtListElse(std::move(stmtListElse)){};
bool IfNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const IfNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         *this->StmtListThen == *casted_other->StmtListThen &&
         *this->StmtListElse == *casted_other->StmtListElse;
};

#pragma once
#include <vector>
#include <memory>
#include <regex>

/*
 * Defines the main constructs for the Query Language PQL
*/

// Grammar for the select clause
// select-cl : declaration* ‘Select’ synonym [ suchthat-cl ] [ pattern-cl ]
// (zero or more declarations), followed by the word Select
namespace QE {
    enum class DesignEntity {
        STMT,
        READ,
        PRINT,
        CALL,
        WHILE,
        IF,
        ASSIGN,
        VARIABLE,
        CONSTANT,
        PROCEDURE
    };
    class Declaration {
        private:
            // Spec: LETTER (LETTER | DIGIT)*
            const std::regex synonym_regex = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");
        public:
            const DesignEntity design_entity;
            const std::string synonym;
            Declaration(DesignEntity de, std::string syn) : design_entity(de), synonym(syn) { };
            bool isValid();
            bool operator==(const Declaration& a2) const {
                return true;
            }
    };

    class SuchThat {

    };

    class Pattern {

    };

    class Query {
        private:
            std::vector<Declaration>* declarations;
            Declaration* selected_declaration;
            // No std::optional (is in C++17 - have to use nullable types)
            SuchThat* such_that;
            Pattern* pattern;
        public:
            // -- Accessors and constructors for Query --
            Query(std::vector<Declaration>*, Declaration*);
            Query();
            ~Query();
            std::vector<Declaration>* getDeclarations() { return declarations; }
            Declaration* getSelectedDeclarations() { return selected_declaration; }
            SuchThat* getSuchThat() { return such_that; }
            Pattern* getPattern() { return pattern; }

            // -- Utility Methods --
            // Make a basic (invalid) query with empty definition and empty select
            static std::unique_ptr<Query> makeStubQuery();
    };
}

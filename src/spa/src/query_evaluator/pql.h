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


    // WARNING: UPDATING THIS CLASS --> THE wordMap in the CPP FILE MUST BE UPDATED
    enum class DesignEntity {
        STMT=1,
        READ,
        PRINT,
        CALL,
        WHILE,
        IF,
        ASSIGN,
        VARIABLE,
        CONSTANT,
        PROCEDURE=10
    };
    // wordMap is declared in the .cpp file

    // Basic accessor for the DesignEntity word map and a reverse mapper
    // Note: can throw exception..really wish i had std::optional
    DesignEntity getDesignEntity(std::string&);
    std::string getDesignEntityString(DesignEntity);

    class Declaration {
        private:
            // Spec: LETTER (LETTER | DIGIT)*
            const std::regex synonym_regex = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");
        public:
            // Members
            const DesignEntity design_entity;
            const std::string synonym;

            // Constructors
            Declaration(DesignEntity de, std::string syn) : design_entity(de), synonym(syn) { };
            // Checks if synonym is valid (and therefore that Declaration is valid)
            bool isValid();
            bool operator==(const Declaration& a2) const {
                return design_entity == a2.design_entity && synonym == a2.synonym;
            }

            friend std::ostream& operator <<(std::ostream& os, Declaration const& decl ) {
                os << getDesignEntityString(decl.design_entity) << " " << decl.synonym;
                return os;
            }

    };

    class SuchThat {

    };

    class Pattern {

    };

    class Query {
        public:
            // -- Data --

            std::vector<Declaration>* declarations;
            // Selected declaration refers to the synonym that is after the 'Select'
            Declaration* selected_declaration;
            // No std::optional (is in C++17 - have to use nullable types)
            SuchThat* such_that;
            Pattern* pattern;

            // -- Accessors and constructors for Query --

            Query(std::vector<Declaration>*, Declaration*);
            Query();
            ~Query();


            // -- Utility Methods --

            // Make a basic (invalid) query with empty definition and empty select
            static std::unique_ptr<Query> makeStubQuery();
    };
}

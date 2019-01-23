#ifndef __QUERY_EVALUATOR_H
#define __QUERY_EVALUATOR_H
#include <string>
#include <memory> // future use of smart pointers

/*
 * The main API for the PQL Query Engine (QE)
 * 
*/

namespace QE {
    // Likely to put this in a shared space with the PKB instead of here
    class Query {
        // Stub class
    };
    class QueryResult {
        // Stub class
    };

    class QueryEvaluator {
        private:
            // Given a string containing PQL, 
            // returns the Query object representing the query's structure
            std::unique_ptr<Query> getQuery(std::string&);
        public:
            // void - placeholder for an unknown return type
            // Give a string containing PQL, makes the appropriate calls to PKB
            // and retrieves the QueryResult
            std::unique_ptr<QueryResult> makePqlQuery(std::string&);

    };
}

#endif
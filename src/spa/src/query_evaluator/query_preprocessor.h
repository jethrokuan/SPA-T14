#ifndef __QUERY_PREPROCESSOR_H
#define __QUERY_PREPROCESSOR_H
#include <string>
#include <memory> // future use of smart pointers
#include "pql.h"

/*
 * Utility class that takes in a PQL query string and returns a Query object
 * for further processing by the Query system
 * 
*/

namespace QE {
    class QueryPreprocessor {
        public:
            // Given a string containing PQL, 
            // returns the Query object representing the query's structure
            std::unique_ptr<Query> getQuery(std::string&);
    };
}

#endif
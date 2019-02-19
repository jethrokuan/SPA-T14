#include "TestWrapper.h"
#include <fstream>
#include "spa_manager/spa_manager.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  spa_manager = new SPAManager();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
  try {
    spa_manager->loadSimpleSource(filename);
    std::cout << "parsed " << filename << std::endl;
  } catch (const std::runtime_error e) {
    std::cout << e.what() << std::endl;
    std::cout << "Failed to parse " << filename << std::endl;
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...
  try {
    auto query_results = spa_manager->query(query);
    if (query_results.empty()) {
      std::cout << "Query Result: NONE\n";
    } else {
      // We have results - return to screen
      std::cout << "Query Results: \n";
      for (unsigned int i = 0; i < query_results.size(); i++) {
        // For autotester
        results.push_back(query_results[i]);
        // For display
        std::cout << query_results[i];
        // Comma addition only for non-last results
        if (i != (query_results.size() - 1)) {
          std::cout << ", ";
        }
      }
    }
  } catch (const std::runtime_error e) {
    std::cout << e.what() << std::endl;
    std::cout << "Failed to parse query " << query << std::endl;
  }
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}

TestWrapper::~TestWrapper() { delete spa_manager; }

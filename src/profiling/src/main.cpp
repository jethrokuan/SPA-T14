#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <ratio>
#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_executor/query_executor.h"
#include "simple_parser/interface.h"
#include "spa_manager/spa_manager.h"

using namespace std::chrono;

duration<double> run_test_iteration() {
  // Initialize AST
  auto spa_manager = new SPAManager();
  spa_manager->loadSimpleSource(
      "tests/system_tests/iteration-3/stress-test/2-source.txt");

  // Run query - get timers
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  auto querystr = std::string(
      "if ifs; variable v1; assign a1, a2, a3, a4, a5; Select BOOLEAN such "
      "that Affects*(a1, a2) and Next*(a2, a3) and Affects*(a3, a4) and "
      "Next*(a4, a5) and Next*(a5, a2) and Affects*(a2, a4) and Affects*(a1, "
      "a5) with v1.varName = \"procedure\" pattern ifs(\"procedure\", _, _) "
      "with 10 = 11 and \"procedure\" = \"procedure\" and 1 = 1");
  auto res = spa_manager->query(querystr);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  for (const auto& r : res) {
    std::cout << r << ",";
  }
  std::cout << "\n";

  delete spa_manager;

  return time_span;
}

int main(void) {
  const int NUM_ITER = 10;
  duration<double> total_time_taken;

  std::cout << "\nSTARTING EXECUTION: " << NUM_ITER << " ITERATIONS.\n\n";

  for (int i = 0; i < NUM_ITER; i++) {
    duration<double> time_taken = run_test_iteration();
    std::cout << time_taken.count() << "\tseconds.\n";
    total_time_taken += time_taken;
  }

  std::cout << "\nTOTAL: \n" << total_time_taken.count() << "\tseconds.\n\n";
  std::cout << "AVERAGE: \n"
            << total_time_taken.count() / NUM_ITER << "\tseconds.\n";
  std::cout << std::endl;

  std::cout << "Profiling run complete.\n";
}
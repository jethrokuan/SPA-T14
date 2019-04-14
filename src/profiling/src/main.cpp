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
      "stmt s1, s2; assign a; while w; if i; procedure p; variable v; read r; "
      "Select <s1, s2, a, w, i, p, v, r> such that Calls(p, \"animals\") and "
      "Modifies(a, v) and Uses(w, v) and Parent(w, i) and Follows(w, s1) and "
      "Next*(s1, s2) pattern a(\"x\", _)");
  spa_manager->query(querystr);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  delete spa_manager;

  return time_span;
}

int main(void) {
  const int NUM_ITER = 1;
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
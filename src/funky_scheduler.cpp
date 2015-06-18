#include "input.hpp"
#include "graph.hpp"
#include "schedule.hpp"

#include <iostream>
#include <thread>

int main()
{
  init_logging();

  input_t in = load(std::cin);
  graph_t g = parse(in);

  schedule_t schedule = find_any_valid(g);

  std::cout << "first found schedule: "
            << fixed << setprecision(2) << rating(schedule, g, in) << endl;
  print(std::cout, schedule, g, in);

  vector<std::thread> threads;
  for (size_t i = 0; i < in.n_threads - 1; ++i)
    threads.emplace_back(search_better, schedule, g, in);

  search_better(schedule, g, in);

  return 0;
}

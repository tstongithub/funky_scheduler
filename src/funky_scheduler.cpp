#include "input.hpp"
#include "graph.hpp"
#include "schedule.hpp"

int main()
{
  init_logging();

  input_t in = load(std::cin);
  graph_t g = parse(in);

  schedule_t s = { 2, 4, 0, 10, 1, 7 };

  print(std::cout, s, g, in);

  return 0;
}

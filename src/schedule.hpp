#pragma once

#include "input.hpp"
#include "graph.hpp"

struct schedule_t
{
  vector<int> t2p;
  vector<int> p2t;
  vector<bitset<MAX_GROUPS>> mask;
};

schedule_t find_any_valid(const graph_t&);
void repair(schedule_t&, const graph_t&);
void mutate(schedule_t&, graph_t&, size_t pct);

void search_better(schedule_t, graph_t, const input_t&);

double rating(const schedule_t&, const graph_t&, const input_t&);
void print(std::ostream&, const schedule_t&, const graph_t&, const input_t&);

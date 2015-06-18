#pragma once

#include "input.hpp"
#include "graph.hpp"

// task index -> proc index
typedef vector<int> schedule_t;

schedule_t find_any_valid(const graph_t&);

float rating(const schedule_t&, const graph_t&, const input_t&);

void print(std::ostream&, const schedule_t&, const graph_t&, const input_t&);

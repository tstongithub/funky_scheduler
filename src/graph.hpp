#pragma once

#include "input.hpp"

struct graph_t
{
  size_t n_groups = 0;
  size_t n_tasks;
  size_t n_procs;

  vector<vector<bool>> can;
  vector<vector<size_t>> p_t;
  vector<vector<size_t>> t_p;

  vector<double> penalty;
};

graph_t parse(const input_t&);

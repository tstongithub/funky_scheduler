#pragma once

#include "common.hpp"

constexpr size_t MAX_GROUPS = 7;

struct student_t
{
  string name;
  string director;
  size_t group;
  vector<string> topics;
};

struct professor_t
{
  string name;
  double rank;
  vector<size_t> cannot_groups;
  vector<string> topics;
};

struct input_t
{
  size_t n_threads;
  size_t mutate_pct;
  double print_threshold;
  double revert_threshold;

  double rank_k;
  double penalty_k;
  vector<double> length_penalties;
  vector<double> window_penalties;

  vector<student_t> students;
  vector<professor_t> professors;
};

input_t load(std::istream&);

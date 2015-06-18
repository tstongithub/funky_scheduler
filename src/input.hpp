#pragma once

#include "common.hpp"

constexpr size_t MAX_GROUPS = 7;

struct student_t
{
  string name;
  vector<string> topics;
  string director;
  vector<size_t> groups;
};

struct professor_t
{
  string name;
  vector<string> topics;
  float rank;
  vector<size_t> cannot_groups;
};

struct input_t
{
  float rank_k;
  float penalty_k;
  vector<float> length_penalties;
  vector<float> window_penalties;

  vector<student_t> students;
  vector<professor_t> professors;
};

input_t load(std::istream&);

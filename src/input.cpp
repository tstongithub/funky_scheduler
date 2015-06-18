#include "input.hpp"

#include <cassert>
#include <yaml-cpp/yaml.h>

input_t load(std::istream& is)
{
  LOG(info) << "loading input";

  YAML::Node node = YAML::Load(is);

  input_t in;

  in.n_threads = node["n_threads"].as<size_t>();
  in.mutate_pct = node["mutate_pct"].as<size_t>();
  in.print_threshold = node["print_threshold"].as<double>();
  in.revert_threshold = node["revert_threshold"].as<double>();

  in.rank_k = node["rank_k"].as<double>();
  in.penalty_k = node["penalty_k"].as<double>();

  in.length_penalties = node["length_penalties"].as<vector<double>>();
  in.window_penalties = node["window_penalties"].as<vector<double>>();

  for (const auto& st_node : node["students"]) {
    student_t st;

    st.name = st_node["name"].as<string>();
    st.director = st_node["director"].as<string>();
    st.group = st_node["group"].as<size_t>();
    st.topics = st_node["topics"].as<vector<string>>();
    std::sort(st.topics.begin(), st.topics.end());

    in.students.push_back(st);
  }

  LOG(info) << in.students.size() << " students loaded";

  for (const auto& pr_node : node["professors"]) {
    professor_t pr;

    pr.name = pr_node["name"].as<string>();
    pr.rank = pr_node["rank"].as<double>();
    pr.cannot_groups = pr_node["cannot_groups"].as<vector<size_t>>();
    std::sort(pr.cannot_groups.begin(), pr.cannot_groups.end());
    pr.topics = pr_node["topics"].as<vector<string>>();
    std::sort(pr.topics.begin(), pr.topics.end());

    in.professors.push_back(pr);
  }

  LOG(info) << in.professors.size() << " professors loaded";

  return in;
}

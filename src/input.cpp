#include "input.hpp"

#include <cassert>
#include <yaml-cpp/yaml.h>

input_t load(std::istream& is)
{
  LOG(info) << "loading input";

  YAML::Node node = YAML::Load(is);

  input_t in;

  in.rank_k = node["rank_k"].as<float>();
  in.penalty_k = node["penalty_k"].as<float>();

  in.length_penalties = node["length_penalties"].as<vector<float>>();
  in.window_penalties = node["window_penalties"].as<vector<float>>();

  for (const auto& st_node : node["students"]) {
    student_t st;

    st.name = st_node["name"].as<string>();
    st.topics = st_node["topics"].as<vector<string>>();
    std::sort(st.topics.begin(), st.topics.end());
    st.director = st_node["director"].as<string>();
    st.groups = st_node["groups"].as<vector<size_t>>();
    assert(st.groups.size() > 0);

    in.students.push_back(st);
  }

  LOG(info) << in.students.size() << " students loaded";

  for (const auto& pr_node : node["professors"]) {
    professor_t pr;

    pr.name = pr_node["name"].as<string>();
    pr.topics = pr_node["topics"].as<vector<string>>();
    std::sort(pr.topics.begin(), pr.topics.end());
    pr.rank = pr_node["rank"].as<float>();
    pr.cannot_groups = pr_node["cannot_groups"].as<vector<size_t>>();
    std::sort(pr.cannot_groups.begin(), pr.cannot_groups.end());

    in.professors.push_back(pr);
  }

  std::sort(in.professors.begin(), in.professors.end(),
            [](const professor_t& p1, const professor_t& p2) {
              return p2.rank < p1.rank;
            });

  LOG(info) << in.professors.size() << " professors loaded";

  return in;
}

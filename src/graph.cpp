#include "graph.hpp"

graph_t parse(const input_t& in)
{
  LOG(info) << "parsing graph";

  graph_t g;

  for (const auto& st : in.students)
    g.n_groups = max(g.n_groups, 1 + *max_element(st.groups.begin(),
                                                  st.groups.end()));

  LOG(info) << g.n_groups << " groups";

  g.n_tasks = 2 * in.students.size();
  g.n_procs = g.n_groups * in.professors.size();

  LOG(info) << g.n_tasks << " tasks";
  LOG(info) << g.n_procs << " processors";

  size_t n_edges = 0;
  for (const auto& pr : in.professors)
    for (size_t gi = 0; gi < g.n_groups; ++gi) {
      vector<bool> cans;

      for (const auto& st : in.students) {
        bool can =
          st.director != pr.name &&
          std::includes(pr.topics.begin(), pr.topics.end(),
                        st.topics.begin(), st.topics.end()) &&
          !std::binary_search(pr.cannot_groups.begin(),
                              pr.cannot_groups.end(),
                              gi);

        if (can) n_edges += 2;

        cans.push_back(can);
        cans.push_back(can);
      }

      g.can.push_back(cans);
    }

  LOG(info) << n_edges << " edges";

  g.p_t.resize(g.n_procs);
  g.t_p.resize(g.n_tasks);
  for (size_t pi = 0; pi < g.n_procs; ++pi)
    for (size_t ti = 0; ti < g.n_tasks; ++ti)
      if (g.can[pi][ti]) {
        g.p_t[pi].push_back(ti);
        g.t_p[ti].push_back(pi);
      }

  for (auto& ps : g.t_p)
    std::sort(ps.begin(), ps.end());

  LOG(debug) << "precalculating penalties";

  g.penalty.resize(1u << g.n_groups);
  g.penalty[0] = 0;
  for (size_t m = 1; m < g.penalty.size(); ++m) {
    bitset<MAX_GROUPS> mask(m);

    size_t first = 0;
    while (!mask[first]) ++first;

    size_t last = g.n_groups - 1;
    while (!mask[last]) --last;

    const size_t length = last - first + 1;
    const float length_penalty = in.length_penalties[length];
    const float window_penalty = in.window_penalties[length - mask.count()];

    g.penalty[m] = in.penalty_k * (length_penalty + window_penalty);

    string str = mask.to_string();
    std::reverse(str.begin(), str.end());

    LOG(debug) << str.substr(0, g.n_groups)
               << " length = " << length
               << " length_penalty = " << setw(5) << fixed << setprecision(2)
               << length_penalty
               << " window_penalty = " << setw(5) << fixed << setprecision(2)
               << window_penalty
               << " penalty = " << setw(5) << fixed << setprecision(2)
               << g.penalty[m];
  }

  return g;
}

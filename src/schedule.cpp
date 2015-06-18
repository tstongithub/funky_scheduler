#include "schedule.hpp"

#include <sstream>

schedule_t find_any_valid(const graph_t&)
{
  return {};
}

float rating(const schedule_t&, const graph_t&, const input_t&)
{
  return 0;
}

void print(std::ostream& out,
           const schedule_t& schedule,
           const graph_t& g,
           const input_t& in)
{
  for (size_t gi = 0; gi < g.n_groups; ++gi)
    out << gi;
  out << " " << setw(9) << "rating" << " | professor => details" << endl;

  assert(schedule.size() == g.n_tasks);

  vector<vector<string>> details(in.professors.size());
  vector<bitset<MAX_GROUPS>> mask(in.professors.size());
  for (size_t ti = 0; ti < g.n_tasks; ++ti) {
    assert(schedule[ti] >= 0);
    const size_t pi = static_cast<size_t>(schedule[ti]);
    const size_t pr_i = pi / g.n_groups;
    const size_t gi = pi % g.n_groups;
    mask[pr_i].set(gi);
    details[pr_i].push_back(std::to_string(gi) + ":" + in.students[ti / 2].name);
  }

  for (size_t pr_i = 0; pr_i < in.professors.size(); ++pr_i)
    if (mask[pr_i].any()) {
      const auto& pr = in.professors[pr_i];
      string str = mask[pr_i].to_string();
      std::reverse(str.begin(), str.end());

      out << str.substr(0, g.n_groups)
          << " " << setw(9) << fixed << setprecision(2)
          << pr.rank * in.rank_k - g.penalty[mask[pr_i].to_ulong()]
          << " | "  << pr.name << " =>";

      auto& d = details[pr_i];
      std::sort(d.begin(), d.end());
      for (const auto& s : d)
        out << " " << s;
      out << endl;
    }
}

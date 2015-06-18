#include "schedule.hpp"

#include <sstream>
#include <unordered_set>

namespace {

  bool find_improving_path(size_t ti,
                           schedule_t& schedule,
                           const graph_t& g,
                           vector<bool>& visited)
  {
    if (visited[ti]) return false;
    visited[ti] = true;
    for (size_t pi : g.t_p[ti]) {
      const int next_ti = schedule.p2t[pi];
      if (next_ti == -1 || find_improving_path(static_cast<size_t>(next_ti),
                                               schedule, g, visited)) {
        schedule.p2t[pi] = ti;
        schedule.t2p[ti] = pi;
        schedule.mask[pi / g.n_groups].set(pi % g.n_groups);
        return true;
      }
    }
    return false;
  }

#ifndef NDEBUG
  bool valid(const schedule_t& schedule, const graph_t& g)
  {
    if (schedule.t2p.size() != g.n_tasks) return false;
    if (schedule.p2t.size() != g.n_procs) return false;
    if (schedule.mask.size() * g.n_groups != g.n_procs) return false;

    for (size_t ti = 0; ti < g.n_tasks; ++ti) {
      const int pi = schedule.t2p[ti];
      if (pi < 0 || pi >= static_cast<int>(g.n_procs)) return false;
      const int back_ti = schedule.p2t[static_cast<size_t>(pi)];
      if (static_cast<size_t>(back_ti) != ti) return false;
      if (!g.can[static_cast<size_t>(pi)][ti]) return false;
    }

    for (size_t pi = 0; pi < g.n_procs; ++pi) {
      const bool used = schedule.p2t[pi] != -1;

      if (schedule.mask[pi / g.n_groups][pi % g.n_groups] != used)
        return false;
    }

    return true;
  }
#endif

  // Marsaglia's xorshf generator
  struct xorshf96_t
  {
    uint64_t x = 123456789;
    uint64_t y = 362436069;
    uint64_t z = 521288629;

    void randseed()
    {
      x = static_cast<uint64_t>(rand() % 1000000000);
      y = static_cast<uint64_t>(rand() % 1000000000);
      z = static_cast<uint64_t>(rand() % 1000000000);
    }

    uint64_t next()
    {
      uint64_t t;
      x ^= x << 16;
      x ^= x >> 5;
      x ^= x << 1;

      t = x;
      x = y;
      y = z;
      z = t ^ x ^ y;

      return z;
    }

   uint64_t operator()(uint64_t base)
   {
     return next() % base;
   }
 };

  thread_local xorshf96_t xorshf96;

  size_t hash(const schedule_t& schedule)
  {
    size_t res = 0;

    for (int pi : schedule.t2p)
      res ^= static_cast<size_t>(pi) + 0x9e3779b9u + (res << 6) + (res >> 2);

    return res;
  }
}

void repair(schedule_t& schedule, const graph_t& g)
{
  vector<bool> visited;
  for (size_t ti = 0; ti < g.n_tasks; ++ti)
    if (schedule.t2p[ti] == -1) {
      visited.assign(g.n_procs, false);
      if (!find_improving_path(ti, schedule, g, visited))
        throw std::runtime_error("failed to repair schedule");
    }
}

schedule_t find_any_valid(const graph_t& g)
{
  schedule_t schedule;
  schedule.t2p.assign(g.n_tasks, -1);
  schedule.p2t.assign(g.n_procs, -1);
  schedule.mask.assign(g.n_procs / g.n_groups, {});

  repair(schedule, g);

  return schedule;
}

void mutate(schedule_t& schedule, graph_t& g, size_t pct)
{
  for (size_t ti = 0; ti < g.n_tasks; ++ti) {
    // remove some part of schedule
    if (xorshf96.next() % 100 < pct) {
      const size_t pi = static_cast<size_t>(schedule.t2p[ti]);
      schedule.t2p[ti] = -1;
      schedule.p2t[pi] = -1;
      schedule.mask[pi / g.n_groups].set(pi % g.n_groups, false);
    }
    // reshuffle edges, so new schedule may be different
    std::random_shuffle(g.t_p[ti].begin(), g.t_p[ti].end(), xorshf96);
  }
  repair(schedule, g);
}

double rating(const schedule_t& schedule, const graph_t& g, const input_t& in)
{
  assert(valid(schedule, g));

  double rating = 0;

  for (size_t st_i = 0; st_i < in.students.size(); ++st_i) {
    const int p1i = schedule.t2p[2 * st_i];
    const size_t pr1_i = static_cast<size_t>(p1i) / g.n_groups;
    const int p2i = schedule.t2p[2 * st_i + 1];
    const size_t pr2_i = static_cast<size_t>(p2i) / g.n_groups;

    const double max_rank = max(in.professors[pr1_i].rank,
                                in.professors[pr2_i].rank);
    const double penalty =
      g.penalty[schedule.mask[pr1_i].to_ulong()] +
      g.penalty[schedule.mask[pr2_i].to_ulong()];

    rating += max_rank * in.rank_k - penalty;
  }

  return rating;
}

void print(std::ostream& out,
           const schedule_t& schedule,
           const graph_t& g,
           const input_t& in)
{
  for (size_t gi = 0; gi < g.n_groups; ++gi)
    out << gi;
  out << " " << setw(9) << "rating" << " | professor => students" << endl;

  for (size_t pr_i = 0; pr_i < in.professors.size(); ++pr_i)
    if (schedule.mask[pr_i].any()) {
      const auto& pr = in.professors[pr_i];
      string str = schedule.mask[pr_i].to_string();
      std::reverse(str.begin(), str.end());

      out << str.substr(0, g.n_groups)
          << " " << setw(9) << fixed << setprecision(2)
          << pr.rank * in.rank_k - g.penalty[schedule.mask[pr_i].to_ulong()]
          << " | "  << pr.name << " =>";

      for (size_t gi = 0; gi < g.n_groups; ++gi) {
        const int ti = schedule.p2t[pr_i * g.n_groups + gi];
        if (ti != -1)
          out << " " << std::to_string(gi) << ":"
              << in.students[static_cast<size_t>(ti) / 2].name;
      }

      out << endl;
    }
}

void search_better(schedule_t best, graph_t g, const input_t& in)
{
  xorshf96.randseed();

  double best_r = rating(best, g, in);

  schedule_t next = best;

  std::unordered_set<size_t> seen;

  while (true) {
    mutate(next, g, in.mutate_pct);
    const double next_r = rating(next, g, in);

    if (next_r / best_r > in.print_threshold) { // good enough, print summary
      const size_t h = hash(next);
      if (seen.find(h) == seen.end()) {
        seen.insert(h);
        std::ostringstream oss;
        oss << fixed << setprecision(2) << next_r << " ";
        for (size_t st_i = 0; st_i < in.students.size(); ++st_i)
          oss << "(" << static_cast<size_t>(next.t2p[st_i * 2]) / g.n_groups
              << "," << static_cast<size_t>(next.t2p[st_i * 2 + 1]) / g.n_groups
              << ")";
        LOG(info) << oss.str();
      }
    }

    if (next_r > best_r) {
      std::ostringstream oss;
      oss << "new best schedule: "
          << fixed << setprecision(2) << next_r << endl;
      print(oss, next, g, in);
      LOG(info) << oss.str();
      best = next;
      best_r = next_r;
    }

    if (next_r / best_r < in.revert_threshold)
      next = best;
  }
}

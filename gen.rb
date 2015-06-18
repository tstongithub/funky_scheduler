#!/usr/bin/env ruby

require 'active_support'
require 'active_support/core_ext'
require 'yaml'

n_students = ARGV[0].to_i
n_groups = ARGV[1].to_i
n_professors = ARGV[2].to_i

res = {
  n_threads: 4,
  mutate_pct: 30,
  print_threshold: 0.99,
  revert_threshold: 0.7,

  rank_k: 1.0,
  penalty_k: 1.0,


  length_penalties: [0, 0, 0, 5, 20, 80, 250, 1000],
  window_penalties: [0, 5, 25, 125, 625, 3000],

  students: [],
  professors: []
}

n_groups.times do |gi|
  n_students.times do |si|
    res[:students] << {
      name: "student_#{gi}_#{si}",
      topics: ['topic1', 'topic2', 'topic3'].select { rand(100) < 20 },
      group: gi,
      director: "professor_#{rand(n_professors)}"
    }
  end
end

n_professors.times do |pi|
  res[:professors] << {
    name: "professor_#{pi}",
    rank: rand(200),
    topics: ['topic1', 'topic2', 'topic3'].reject { rand(100) < 20 },
    cannot_groups: (1..n_groups).select { rand(100) < 20 }
  }
end

puts YAML::dump(res.deep_stringify_keys)

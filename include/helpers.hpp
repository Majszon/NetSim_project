// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif  //HELPERS_HPP_
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

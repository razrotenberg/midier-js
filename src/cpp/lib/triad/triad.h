#pragma once

#include <vector>

#include "midier/degree/degree.h"
#include "midier/interval/interval.h"
#include "midier/quality/quality.h"

namespace midier
{
namespace triad
{

Interval interval(Quality quality, Degree degree);

} // triad

struct Chord
{
    Interval interval(Degree degree) const;

 private:
    std::vector<Interval> _intervals;
};

} // midier

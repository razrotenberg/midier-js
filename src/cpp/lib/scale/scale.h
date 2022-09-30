#pragma once

#include <string>
#include <vector>

#include "midier/degree/degree.h"
#include "midier/interval/interval.h"
#include "midier/mode/mode.h"
#include "midier/quality/quality.h"
#include "midier/triad/triad.h"

namespace midier
{
namespace scale
{

Interval interval(Mode mode, Degree degree);
Quality  quality (Mode mode, Degree degree);

} // scale

struct Scale
{
    struct Degree
    {
        Interval interval;
        Chord chord;
    };

    // creation
    Scale(const std::string & name, const std::vector<Degree> & degrees);

    // queries
    const std::string & name() const;
    unsigned degrees() const;

    // getters
    Degree degree(midier::Degree degree) const;

 private:
    std::string _name;
    std::vector<Degree> _degrees;
};

} // midier

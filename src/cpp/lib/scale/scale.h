#pragma once

#include "midier/degree/degree.h"
#include "midier/interval/interval.h"
#include "midier/mode/mode.h"
#include "midier/quality/quality.h"

namespace midier
{
namespace scale
{

Interval interval(Mode mode, Degree degree);
Quality  quality (Mode mode, Degree degree);

} // scale
} // midier

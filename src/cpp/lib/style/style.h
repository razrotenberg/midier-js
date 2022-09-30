#pragma once

#include "midier/degree/degree.h"

namespace midier
{
namespace style
{

// think about this

Degree degree(unsigned n, unsigned r, unsigned index);
unsigned count(unsigned n);

using Description = char[13]; // 12 + '\0'
void description(unsigned n, unsigned r, /* out */ Description & desc);

} // style
} // midier

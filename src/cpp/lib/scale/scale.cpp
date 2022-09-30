#include "midier/scale/scale.h"

namespace midier
{
namespace scale
{

#define ASSERT(mode, expected) static_assert(static_cast<int>(mode) == (expected), "Expected midier::" #mode " to be equal to " #expected);

ASSERT(Mode::Ionian,     0);
ASSERT(Mode::Dorian,     1);
ASSERT(Mode::Phrygian,   2);
ASSERT(Mode::Lydian,     3);
ASSERT(Mode::Mixolydian, 4);
ASSERT(Mode::Aeolian,    5);
ASSERT(Mode::Locrian,    6);

Interval interval(Mode mode, Degree degree)
{
    static Interval const __ionian     [] = { Interval::P1, Interval::M2, Interval::M3, Interval::P4, Interval::P5, Interval::M6, Interval::M7 };
    static Interval const __dorian     [] = { Interval::P1, Interval::M2, Interval::m3, Interval::P4, Interval::P5, Interval::M6, Interval::m7 };
    static Interval const __phrygian   [] = { Interval::P1, Interval::m2, Interval::m3, Interval::P4, Interval::P5, Interval::m6, Interval::m7 };
    static Interval const __lydian     [] = { Interval::P1, Interval::M2, Interval::M3, Interval::A4, Interval::P5, Interval::M6, Interval::M7 };
    static Interval const __mixolydian [] = { Interval::P1, Interval::M2, Interval::M3, Interval::P4, Interval::P5, Interval::M6, Interval::m7 };
    static Interval const __aeolian    [] = { Interval::P1, Interval::M2, Interval::m3, Interval::P4, Interval::P5, Interval::m6, Interval::m7 };
    static Interval const __locrian    [] = { Interval::P1, Interval::m2, Interval::m3, Interval::P4, Interval::d5, Interval::m6, Interval::m7 };

    static Interval const * const __all[] =
        {
            __ionian,
            __dorian,
            __phrygian,
            __lydian,
            __mixolydian,
            __aeolian,
            __locrian,
        };

    static_assert(sizeof(__all) / sizeof(__all[0]) == (unsigned)Mode::Count, "Unexpected number of modes declared");

    Interval octaver = Interval::P1;

    while (degree > 7)
    {
        degree -= 7;
        octaver = octaver + Interval::P8;
    }

    return octaver + __all[(unsigned)mode][degree - 1];
}

Quality quality(Mode mode, Degree degree)
{
    static Quality const __qualities[] = { Quality::maj7, Quality::m7, Quality::m7, Quality::maj7, Quality::dom7, Quality::m7, Quality::m7b5 };

    constexpr auto __count = sizeof(__qualities) / sizeof(__qualities[0]);

    static_assert(__count == 7, "Expected 7 qualities to be declared");

    return __qualities[(degree - 1 + (unsigned)mode) % __count];
}

} // scale

Scale::Scale(const std::string & name, const std::vector<Degree> & degrees) :
    _name(name),
    _degrees(degrees)
{}

const std::string & Scale::name() const
{
    return _name;
}

unsigned Scale::degrees() const
{
    return _degrees.size();
}

Interval Scale::interval(midier::Degree degree) const
{
    Interval octaver = Interval::P1;

    while (degree > 7)
    {
        degree -= 7;
        octaver = octaver + Interval::P8;
    }

    return octaver + _degrees.at(degree - 1).interval;
}

const Scale::Degree & Scale::degree(midier::Degree degree) const
{
    return _degrees.at(degree - 1);
}

} // midier

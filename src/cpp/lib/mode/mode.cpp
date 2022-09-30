#include "midier/mode/mode.h"

#include <string.h>

namespace midier
{
namespace mode
{

namespace
{

// remove this
const Name __ionian = "Ionian";
const Name __dorian = "Dorian";
const Name __phrygian = "Phrygian";
const Name __lydian = "Lydian";
const Name __mixolydian = "Mixolydian";
const Name __aeolian = "Aeolian";
const Name __locrian = "Locrian";

char const * const __names[] = {
    __ionian,
    __dorian,
    __phrygian,
    __lydian,
    __mixolydian,
    __aeolian,
    __locrian,
};

static_assert(sizeof(__names) / sizeof(__names[0]) == (unsigned)Mode::Count, "Unexpected number of names declared");

} //

void name(Mode mode, /* out */ Name & name)
{
    strcpy(/* out */ name, __names[(unsigned)mode]);
}

} // mode
} // midier

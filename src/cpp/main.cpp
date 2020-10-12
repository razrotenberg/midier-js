#include <emscripten.h>

#include <vector>
#include <iomanip>
#include <iostream>

#include "midier/sequencer/sequencer.h"

namespace midier
{
namespace midi
{

decltype(&midier::midi::on) __on = nullptr;
decltype(&midier::midi::off) __off = nullptr;

void on(Number number, Velocity velocity)
{
    if (__on != nullptr)
    {
        __on(number, velocity);
    }
}

void off(Number number)
{
    if (__off != nullptr)
    {
        __off(number);
    }
}

} // midi
} // midier

namespace midierjs
{

struct Sequencer
{
    Sequencer(unsigned layers) :
        _layers(layers),
        _ilayers(_layers.data(), _layers.size()),
        sequencer(_ilayers)
    {}

 private:
    std::vector<midier::Layer> _layers;
    midier::ILayers _ilayers;

 public:
    midier::Sequencer sequencer;

 private:
    static std::vector<Sequencer *> _all;

 public:
    static void enroll(Sequencer * sequencer)
    {
        _all.push_back(sequencer);
    }

    static const std::vector<Sequencer *> & all()
    {
        return _all;
    }
};

struct Layer
{
    Layer(midier::Sequencer & sequencer, midier::Degree degree) :
        sequencer(&sequencer),
        handle(sequencer.start(degree))
    {}

    ~Layer()
    {
        sequencer->stop(handle);
    }

    midier::Sequencer * sequencer;
    midier::Sequencer::Handle handle;
};

extern "C" EMSCRIPTEN_KEEPALIVE void onMIDINoteOn(decltype(&midier::midi::on) handler)
{
    midier::midi::__on = handler;
}

extern "C" EMSCRIPTEN_KEEPALIVE void onMIDINoteOff(decltype(&midier::midi::off) handler)
{
    midier::midi::__off = handler;
}

extern "C" EMSCRIPTEN_KEEPALIVE Sequencer * createSequencer(unsigned layers)
{
    // create a new sequencer
    const auto sequencer = new Sequencer(layers);

    // register it
    Sequencer::enroll(sequencer);

    // return it to the user
    return sequencer;
}

extern "C" EMSCRIPTEN_KEEPALIVE Layer * startLayer(Sequencer * sequencer, unsigned degree)
{
    return new Layer(sequencer->sequencer, degree);
}

extern "C" EMSCRIPTEN_KEEPALIVE void stopLayer(Layer * layer)
{
    delete layer;
}

struct Callback
{
    using Function = void(*)();

    Function callback;
    unsigned subdivisions; // how many more subdivisions
};

std::vector<Callback> __callbacks;

extern "C" EMSCRIPTEN_KEEPALIVE void scheduleCallback(Callback::Function callback, float bars)
{
    const auto cb = Callback
        {
            .callback = callback,
            .subdivisions = midier::Time::Duration(bars).total(),
        };

    // specific support to the uncommon case of callbacks that need
    // to be evaluated now, because callbacks are evaluated after clicking
    // Midier, so a zero-subdivision callback cannot be treated normally

    if (cb.subdivisions == 0)
    {
        // std::cout << "Calling callback " << (void*)callback << " now" << std::endl;
        cb.callback();
    }
    else
    {
        // std::cout << "Scheduling callback " << (void*)callback << " in " << bars << " bars (" << cb.subdivisions << " subdivisions)" << std::endl;
        __callbacks.push_back(cb);
    }
}

unsigned __bpm;

extern "C" EMSCRIPTEN_KEEPALIVE unsigned getBPM()
{
    return __bpm;
}

extern "C" EMSCRIPTEN_KEEPALIVE bool setBPM(unsigned bpm)
{
    __bpm = bpm;

    const auto bps = (float)__bpm / 60.f; // beats per second
    const auto mspb = 1000.f / bps; // ms per beat
    const auto mspc = mspb / (float)midier::Time::Subdivisions; // ms per click

    std::cout << "Setting BPM to " << bpm << " (" << std::setprecision(2) << mspc << " ms per click)" << std::endl;

    return emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, mspc) == 0;
}

extern "C" int main()
{
    emscripten_set_main_loop([]()
        {
            for (auto & sequencer : Sequencer::all())
            {
                sequencer->sequencer.click();
            }

            midier::Time::click();

            for (auto it = __callbacks.begin(); it != __callbacks.end();)
            {
                if (--it->subdivisions == 0)
                {
                    it->callback();
                    it = __callbacks.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }, /* fps = */ 0, /* simulate_infinite_loop = */ 0);

    setBPM(60);

    return EXIT_SUCCESS;
}

} // namespace midierjs
